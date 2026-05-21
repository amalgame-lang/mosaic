/* mosaic-supervise.c — worker supervisor for `mosaic dev` v0.5.
 *
 * One supervisor + one running worker at a time.  When the
 * supervisor reads `reload\n` on stdin (sent by mosaic-dev.sh
 * after a successful rebuild), it spawns a NEW worker, waits a
 * short overlap window so the kernel routes a few connections
 * to the new one, then SIGTERMs the OLD worker.  The OLD worker
 * stops accepting but keeps serving its in-flight handler
 * frames + open WebSocket connections until they close (or a
 * 60-second grace timeout, then SIGKILL).
 *
 * The workers themselves bind the port with SO_REUSEPORT, so the
 * kernel load-balances incoming connections during the overlap.
 * The supervisor never touches the listening socket — it's just
 * a process orchestrator.
 *
 * Usage:
 *   mosaic-supervise --binary ./server [--grace 60] [--overlap 250]
 *
 *   reload commands arrive on stdin, one per line.  EOF on stdin
 *   triggers a clean shutdown.
 *
 * Build:
 *   gcc -O2 -Wall mosaic-supervise.c -o mosaic-supervise
 *
 * Design: docs/proposals/hot-reload.md (Approach B).
 */

#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static const char* g_binary  = NULL;
static int         g_grace_s = 60;
static int         g_overlap_ms = 250;
static pid_t       g_worker_pid = 0;

static void usage(void) {
    fprintf(stderr,
        "mosaic-supervise — worker supervisor for `mosaic dev` v0.5+\n"
        "\n"
        "Usage:\n"
        "  mosaic-supervise --binary <path> [--grace <s>] [--overlap <ms>]\n"
        "\n"
        "Reads `reload\\n` commands from stdin to swap the running worker.\n");
}

/* Spawn the worker.  Returns the child pid, or -1 on fork failure. */
static pid_t spawn_worker(void) {
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "[supervise] fork failed: %s\n", strerror(errno));
        return -1;
    }
    if (pid == 0) {
        /* Child: become the worker. */
        char* const argv[] = { (char*) g_binary, NULL };
        execv(g_binary, argv);
        fprintf(stderr, "[supervise] exec(%s) failed: %s\n",
                g_binary, strerror(errno));
        _exit(127);
    }
    fprintf(stderr, "[supervise] spawned worker pid=%d\n", (int) pid);
    return pid;
}

/* Send SIGTERM, then wait up to g_grace_s seconds for a clean
 * exit before sending SIGKILL.  Polls every 100 ms via WNOHANG. */
static void drain_worker(pid_t pid) {
    if (pid <= 0) return;
    fprintf(stderr, "[supervise] SIGTERM pid=%d (grace %ds)\n",
            (int) pid, g_grace_s);
    if (kill(pid, SIGTERM) < 0 && errno != ESRCH) {
        fprintf(stderr, "[supervise] kill(SIGTERM) failed: %s\n",
                strerror(errno));
    }
    struct timespec poll = { 0, 100 * 1000 * 1000 };  /* 100 ms */
    int polls = g_grace_s * 10;
    for (int i = 0; i < polls; i++) {
        int status = 0;
        pid_t r = waitpid(pid, &status, WNOHANG);
        if (r == pid) {
            fprintf(stderr, "[supervise] worker pid=%d exited cleanly\n",
                    (int) pid);
            return;
        }
        if (r < 0) return;
        nanosleep(&poll, NULL);
    }
    fprintf(stderr, "[supervise] worker pid=%d outlasted grace, SIGKILL\n",
            (int) pid);
    kill(pid, SIGKILL);
    waitpid(pid, NULL, 0);
}

/* Handle SIGCHLD so the supervisor doesn't accumulate zombies
 * when a worker exits on its own. */
static void on_sigchld(int sig) {
    (void) sig;
    int status = 0;
    while (waitpid(-1, &status, WNOHANG) > 0) { /* reap */ }
}

/* On SIGINT/SIGTERM: kill the worker (if any) and exit. */
static void on_sigterm(int sig) {
    if (g_worker_pid > 0) {
        kill(g_worker_pid, SIGTERM);
        waitpid(g_worker_pid, NULL, 0);
    }
    fprintf(stderr, "[supervise] received signal %d, exiting\n", sig);
    _exit(0);
}

/* Spawn the new worker, sleep the overlap window, then SIGTERM
 * the old one.  Returns 0 on success, -1 on spawn failure (old
 * worker is left running). */
static int do_reload(void) {
    fprintf(stderr, "[supervise] reload — spawning new worker\n");
    pid_t new_pid = spawn_worker();
    if (new_pid < 0) {
        fprintf(stderr, "[supervise] reload aborted (spawn failed)\n");
        return -1;
    }
    struct timespec overlap = {
        g_overlap_ms / 1000,
        (g_overlap_ms % 1000) * 1000 * 1000
    };
    nanosleep(&overlap, NULL);

    /* Did the new worker crash on startup?  If yes, abort the
     * reload and keep the old worker running. */
    int status = 0;
    pid_t r = waitpid(new_pid, &status, WNOHANG);
    if (r == new_pid) {
        fprintf(stderr,
            "[supervise] new worker pid=%d died during overlap "
            "(exit %d) — keeping old worker pid=%d\n",
            (int) new_pid,
            WIFEXITED(status) ? WEXITSTATUS(status) : -1,
            (int) g_worker_pid);
        return -1;
    }

    pid_t old_pid = g_worker_pid;
    g_worker_pid = new_pid;
    drain_worker(old_pid);
    return 0;
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--binary") == 0 && i + 1 < argc) {
            g_binary = argv[++i];
        } else if (strcmp(argv[i], "--grace") == 0 && i + 1 < argc) {
            g_grace_s = atoi(argv[++i]);
            if (g_grace_s < 1) g_grace_s = 1;
        } else if (strcmp(argv[i], "--overlap") == 0 && i + 1 < argc) {
            g_overlap_ms = atoi(argv[++i]);
            if (g_overlap_ms < 0) g_overlap_ms = 0;
        } else if (strcmp(argv[i], "-h") == 0 ||
                   strcmp(argv[i], "--help") == 0) {
            usage();
            return 0;
        } else {
            fprintf(stderr, "[supervise] unknown arg: %s\n", argv[i]);
            usage();
            return 2;
        }
    }
    if (!g_binary) {
        fprintf(stderr, "[supervise] --binary <path> is required\n");
        usage();
        return 2;
    }
    if (access(g_binary, X_OK) != 0) {
        fprintf(stderr, "[supervise] binary not executable: %s (%s)\n",
                g_binary, strerror(errno));
        return 2;
    }

    /* Reap zombies + propagate signals. */
    struct sigaction sa = {0};
    sa.sa_handler = on_sigchld;
    sa.sa_flags   = SA_NOCLDSTOP | SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);

    struct sigaction st = {0};
    st.sa_handler = on_sigterm;
    sigaction(SIGTERM, &st, NULL);
    sigaction(SIGINT,  &st, NULL);

    g_worker_pid = spawn_worker();
    if (g_worker_pid < 0) {
        fprintf(stderr, "[supervise] initial spawn failed, aborting\n");
        return 1;
    }

    /* Read reload commands from stdin.  EOF triggers clean exit. */
    char buf[64];
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        /* Strip trailing newline. */
        size_t n = strlen(buf);
        while (n > 0 && (buf[n-1] == '\n' || buf[n-1] == '\r')) {
            buf[--n] = '\0';
        }
        if (strcmp(buf, "reload") == 0) {
            do_reload();
        } else if (strcmp(buf, "quit") == 0) {
            break;
        } else if (n > 0) {
            fprintf(stderr, "[supervise] unknown command: %s\n", buf);
        }
    }

    if (g_worker_pid > 0) {
        kill(g_worker_pid, SIGTERM);
        waitpid(g_worker_pid, NULL, 0);
    }
    fprintf(stderr, "[supervise] exiting cleanly\n");
    return 0;
}
