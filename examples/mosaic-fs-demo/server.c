#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "_runtime.h"
#include "Amalgame_String.h"
#include "Amalgame_Collections.h"
#include "Amalgame_IO.h"
#include "Amalgame_Net.h"
#include "Amalgame_Console.h"
#include "Amalgame_Process.h"
#include "/home/neitsab/.amalgame/packages/github.com/amalgame-lang/amalgame-crypto/v0.3.0_844c2870/facade-stub.h"
#include "/home/neitsab/.amalgame/packages/github.com/amalgame-lang/amalgame-net-http/v0.9.6_4d8e9d69/runtime/Amalgame_Net_Http.h"
#include "/home/neitsab/.amalgame/packages/github.com/amalgame-lang/amalgame-datetime/v0.1.0_c6da2c33/facade-stub.h"
#include "/home/neitsab/.amalgame/packages/github.com/amalgame-lang/amalgame-random/v0.1.0_da4ff0f7/facade-stub.h"
#include "/home/neitsab/.amalgame/packages/github.com/amalgame-lang/amalgame-logging/v0.1.0_b0f23a6d/facade-stub.h"
#include "/home/neitsab/.amalgame/packages/github.com/amalgame-lang/amalgame-database-nosql-redis/v0.2.0_93104295/runtime/Amalgame_Database_Redis.h"
#include "/home/neitsab/.amalgame/packages/github.com/amalgame-lang/amalgame-threading/v0.1.0_bfb5d018/runtime/Amalgame_Threading.h"
#include "/home/neitsab/.amalgame/packages/github.com/amalgame-lang/amalgame-web/v0.13.3_a4ca16b2/runtime/Amalgame_Web.h"

typedef struct _Amalgame_Crypto_Sha256 Amalgame_Crypto_Sha256; /* pkg-class */
typedef struct _Amalgame_Crypto_Hmac Amalgame_Crypto_Hmac; /* pkg-class */
typedef struct _Amalgame_Crypto_Aead Amalgame_Crypto_Aead; /* pkg-class */
typedef struct _Amalgame_Crypto_AeadResult Amalgame_Crypto_AeadResult; /* pkg-class */
typedef struct _Amalgame_Crypto_Bytes Amalgame_Crypto_Bytes; /* pkg-class */
typedef struct _Amalgame_Crypto_JwsKey Amalgame_Crypto_JwsKey; /* pkg-class */
typedef struct _Amalgame_Net_Http_HttpRequest Amalgame_Net_Http_HttpRequest; /* pkg-class */
typedef struct _Amalgame_Net_Http_HttpResponse Amalgame_Net_Http_HttpResponse; /* pkg-class */
typedef struct _Amalgame_Net_Http_HttpServer Amalgame_Net_Http_HttpServer; /* pkg-class */
typedef struct _Amalgame_Net_Http_HttpClient Amalgame_Net_Http_HttpClient; /* pkg-class */
typedef struct _Amalgame_Net_Http_Cookie Amalgame_Net_Http_Cookie; /* pkg-class */
typedef struct _Amalgame_Net_Http_HttpRequestBuilder Amalgame_Net_Http_HttpRequestBuilder; /* pkg-class */
typedef struct _Amalgame_Net_Http_H1Server Amalgame_Net_Http_H1Server; /* pkg-class */
typedef struct _Amalgame_Net_Http_H1Conn Amalgame_Net_Http_H1Conn; /* pkg-class */
typedef struct _Amalgame_Net_Http_Http1 Amalgame_Net_Http_Http1; /* pkg-class */
typedef struct _Amalgame_Net_Http_H2Server Amalgame_Net_Http_H2Server; /* pkg-class */
typedef struct _Amalgame_Net_Http_H2Conn Amalgame_Net_Http_H2Conn; /* pkg-class */
typedef struct _Amalgame_Net_Http_H2 Amalgame_Net_Http_H2; /* pkg-class */
typedef struct _Amalgame_Net_Http_Http2 Amalgame_Net_Http_Http2; /* pkg-class */
typedef struct _Amalgame_Net_Http_HttpsServer Amalgame_Net_Http_HttpsServer; /* pkg-class */
typedef struct _Amalgame_Net_Http_Https Amalgame_Net_Http_Https; /* pkg-class */
typedef struct _Amalgame_Net_Http_WsServer Amalgame_Net_Http_WsServer; /* pkg-class */
typedef struct _Amalgame_Net_Http_WsConn Amalgame_Net_Http_WsConn; /* pkg-class */
typedef struct _Amalgame_Net_Http_Ws Amalgame_Net_Http_Ws; /* pkg-class */
typedef struct _Amalgame_Net_Http_WssServer Amalgame_Net_Http_WssServer; /* pkg-class */
typedef struct _Amalgame_Net_Http_Wss Amalgame_Net_Http_Wss; /* pkg-class */
typedef struct _Amalgame_Net_Http_HttpServerConfig Amalgame_Net_Http_HttpServerConfig; /* pkg-class */
typedef struct _Amalgame_DateTime Amalgame_DateTime; /* pkg-class */
typedef struct _Amalgame_Random Amalgame_Random; /* pkg-class */
typedef struct _Amalgame_Logging_Log Amalgame_Logging_Log; /* pkg-class */
typedef struct _Amalgame_Database_NoSQL_Redis Amalgame_Database_NoSQL_Redis; /* pkg-class */
typedef struct _Amalgame_Threading Amalgame_Threading; /* pkg-class */
typedef struct _Amalgame_Web_Router Amalgame_Web_Router; /* pkg-class */
typedef struct _Amalgame_Web_Route Amalgame_Web_Route; /* pkg-class */
typedef struct _Amalgame_Web_RouteMatch Amalgame_Web_RouteMatch; /* pkg-class */
typedef struct _Amalgame_Web_Session Amalgame_Web_Session; /* pkg-class */
typedef struct _Amalgame_Web_MemorySessionStore Amalgame_Web_MemorySessionStore; /* pkg-class */
typedef struct _Amalgame_Web_WebContext Amalgame_Web_WebContext; /* pkg-class */
typedef struct _Amalgame_Web_SecurityHeaders Amalgame_Web_SecurityHeaders; /* pkg-class */
typedef struct _Amalgame_Web_Cors Amalgame_Web_Cors; /* pkg-class */
typedef struct _Amalgame_Web_RateLimitBucket Amalgame_Web_RateLimitBucket; /* pkg-class */
typedef struct _Amalgame_Web_RateLimit Amalgame_Web_RateLimit; /* pkg-class */
typedef struct _Amalgame_Web_Csrf Amalgame_Web_Csrf; /* pkg-class */
typedef struct _Amalgame_Web_WebApp Amalgame_Web_WebApp; /* pkg-class */
typedef struct _Amalgame_Web_LogConfig Amalgame_Web_LogConfig; /* pkg-class */
typedef struct _Amalgame_Web_SignedCookieSessionStore Amalgame_Web_SignedCookieSessionStore; /* pkg-class */
typedef struct _Amalgame_Web_RedisSessionStore Amalgame_Web_RedisSessionStore; /* pkg-class */
typedef struct _Amalgame_Web_AcmeConfig Amalgame_Web_AcmeConfig; /* pkg-class */
typedef struct _Amalgame_Web_TlsBindingConfig Amalgame_Web_TlsBindingConfig; /* pkg-class */
typedef struct _Amalgame_Web_Static Amalgame_Web_Static; /* pkg-class */
typedef struct _App_Program App_Program;
typedef struct _App_About_Page App_About_Page;
typedef struct _App_Api_Info_Page App_Api_Info_Page;
typedef struct _App_Index_Page App_Index_Page;
typedef struct _App_Users_Id_Page App_Users_Id_Page;
typedef struct _App_Routes App_Routes;
typedef struct _Amalgame_Net_Http_Cookie Amalgame_Net_Http_Cookie; /* external */
typedef struct _Amalgame_Net_Http_HttpRequest Amalgame_Net_Http_HttpRequest; /* external */
typedef struct _Amalgame_Net_Http_HttpResponse Amalgame_Net_Http_HttpResponse; /* external */
typedef struct _Amalgame_Net_Http_HttpParser Amalgame_Net_Http_HttpParser; /* external */
typedef struct _Amalgame_Net_Http_HttpServer Amalgame_Net_Http_HttpServer; /* external */
typedef struct _Amalgame_Net_Http_HttpRequestBuilder Amalgame_Net_Http_HttpRequestBuilder; /* external */
typedef struct _Amalgame_Net_Http_ParsedUrl Amalgame_Net_Http_ParsedUrl; /* external */
typedef struct _Amalgame_Net_Http_HttpClient Amalgame_Net_Http_HttpClient; /* external */
typedef struct _Amalgame_Web_Route Amalgame_Web_Route; /* external */
typedef struct _Amalgame_Web_RouteMatch Amalgame_Web_RouteMatch; /* external */
typedef struct _Amalgame_Web_Router Amalgame_Web_Router; /* external */
typedef struct _Amalgame_Web_Session Amalgame_Web_Session; /* external */
typedef struct _Amalgame_Web_MemorySessionStore Amalgame_Web_MemorySessionStore; /* external */
typedef struct _Amalgame_Web_WebContext Amalgame_Web_WebContext; /* external */
typedef struct _Amalgame_Web_SecurityHeaders Amalgame_Web_SecurityHeaders; /* external */
typedef struct _Amalgame_Web_Cors Amalgame_Web_Cors; /* external */
typedef struct _Amalgame_Web_RateLimitBucket Amalgame_Web_RateLimitBucket; /* external */
typedef struct _Amalgame_Web_RateLimit Amalgame_Web_RateLimit; /* external */
typedef struct _Amalgame_Web_Csrf Amalgame_Web_Csrf; /* external */
typedef struct _Amalgame_Web_LogConfig Amalgame_Web_LogConfig; /* external */
typedef struct _Amalgame_Web_SignedCookieSessionStore Amalgame_Web_SignedCookieSessionStore; /* external */
typedef struct _Amalgame_Web_RedisSessionStore Amalgame_Web_RedisSessionStore; /* external */
typedef struct _Amalgame_Web_AcmeConfig Amalgame_Web_AcmeConfig; /* external */
typedef struct _Amalgame_Web_TlsBindingConfig Amalgame_Web_TlsBindingConfig; /* external */
typedef struct _Amalgame_Web_Static Amalgame_Web_Static; /* external */
typedef struct _Amalgame_Web_WebApp Amalgame_Web_WebApp; /* external */
typedef struct _Amalgame_Crypto_Sha256 Amalgame_Crypto_Sha256; /* external */
typedef struct _Amalgame_Crypto_Hmac Amalgame_Crypto_Hmac; /* external */
typedef struct _Amalgame_Crypto_Bytes Amalgame_Crypto_Bytes; /* external */
typedef struct _Amalgame_Crypto_AeadResult Amalgame_Crypto_AeadResult; /* external */
typedef struct _Amalgame_Crypto_Aead Amalgame_Crypto_Aead; /* external */
typedef struct _Amalgame_Crypto_JwsKey Amalgame_Crypto_JwsKey; /* external */
typedef struct _Amalgame_DateTime_DateTime Amalgame_DateTime_DateTime; /* external */
typedef struct _Amalgame_DateTime_DateTimeUtil Amalgame_DateTime_DateTimeUtil; /* external */
typedef struct _Amalgame_DateTime_Duration Amalgame_DateTime_Duration; /* external */
typedef struct _Amalgame_DateTime_InstantResult Amalgame_DateTime_InstantResult; /* external */
typedef struct _Amalgame_DateTime_Instant Amalgame_DateTime_Instant; /* external */
typedef struct _Amalgame_DateTime_Stopwatch Amalgame_DateTime_Stopwatch; /* external */
typedef struct _Amalgame_Random_Random Amalgame_Random_Random; /* external */
typedef struct _Amalgame_Logging_Log Amalgame_Logging_Log; /* external */
typedef enum _Amalgame_Formats_Json_JsonKind {
    Amalgame_Formats_Json_JsonKind_Null,
    Amalgame_Formats_Json_JsonKind_Bool,
    Amalgame_Formats_Json_JsonKind_Int,
    Amalgame_Formats_Json_JsonKind_Float,
    Amalgame_Formats_Json_JsonKind_String,
    Amalgame_Formats_Json_JsonKind_Array,
    Amalgame_Formats_Json_JsonKind_Object
} Amalgame_Formats_Json_JsonKind; /* external */
typedef struct _Amalgame_Formats_Json_JsonValue Amalgame_Formats_Json_JsonValue; /* external */
typedef struct _Amalgame_Formats_Json_JsonError Amalgame_Formats_Json_JsonError; /* external */
typedef struct _Amalgame_Formats_Json_JsonResult Amalgame_Formats_Json_JsonResult; /* external */
typedef struct _Amalgame_Formats_Json_JsonParser Amalgame_Formats_Json_JsonParser; /* external */
typedef struct _Amalgame_Formats_Json_Json Amalgame_Formats_Json_Json; /* external */
struct _Amalgame_Net_Http_Cookie {
    code_string Name;
    code_string Value;
    code_string Domain;
    code_string Path;
    i64 MaxAgeSec;
    code_bool HttpOnly;
    code_bool Secure;
    code_string SameSite;
};
Amalgame_Net_Http_Cookie* Amalgame_Net_Http_Cookie_new();
code_string Amalgame_Net_Http_Cookie_ToHeader(Amalgame_Net_Http_Cookie* self);
struct _Amalgame_Net_Http_HttpRequest {
    code_string Method;
    code_string Path;
    code_string RawTarget;
    code_string HttpVersion;
    AmalgameMap* Headers;
    AmalgameMap* Query;
    code_string Body;
    code_string RemoteAddr;
    code_bool IsSecure;
};
Amalgame_Net_Http_HttpRequest* Amalgame_Net_Http_HttpRequest_new();
code_string Amalgame_Net_Http_HttpRequest_Header(Amalgame_Net_Http_HttpRequest* self, code_string name);
AmalgameMap* Amalgame_Net_Http_HttpRequest_Form(Amalgame_Net_Http_HttpRequest* self);
code_string Amalgame_Net_Http_HttpRequest_BodyText(Amalgame_Net_Http_HttpRequest* self);
code_string Amalgame_Net_Http_HttpRequest_Cookie(Amalgame_Net_Http_HttpRequest* self, code_string name);
AmalgameMap* Amalgame_Net_Http_HttpRequest_ParseQuery(code_string s);
code_string Amalgame_Net_Http_HttpRequest_UrlDecode(code_string s);
i64 Amalgame_Net_Http_HttpRequest_HexDigit(code_string c);
Amalgame_Net_Http_HttpRequest* Amalgame_Net_Http_HttpRequest_FromH1Conn(i64 conn);
struct _Amalgame_Net_Http_HttpResponse {
    i64 StatusCode;
    AmalgameMap* Headers;
    code_string BodyText;
    code_string FilePath;
};
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpResponse_new();
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpResponse_New();
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpResponse_Status(Amalgame_Net_Http_HttpResponse* self, i64 code);
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpResponse_Header(Amalgame_Net_Http_HttpResponse* self, code_string name, code_string value);
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpResponse_HeaderUnsafe(Amalgame_Net_Http_HttpResponse* self, code_string name, code_string value);
code_bool Amalgame_Net_Http_HttpResponse_HeaderValueHasControl(code_string s);
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpResponse_Text(Amalgame_Net_Http_HttpResponse* self, code_string s);
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpResponse_Html(Amalgame_Net_Http_HttpResponse* self, code_string s);
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpResponse_Json(Amalgame_Net_Http_HttpResponse* self, code_string jsonText);
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpResponse_Redirect(Amalgame_Net_Http_HttpResponse* self, code_string url, code_bool permanent);
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpResponse_SetCookie(Amalgame_Net_Http_HttpResponse* self, Amalgame_Net_Http_Cookie* c);
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpResponse_File(Amalgame_Net_Http_HttpResponse* self, code_string path);
code_string Amalgame_Net_Http_HttpResponse_Render(Amalgame_Net_Http_HttpResponse* self);
code_string Amalgame_Net_Http_HttpResponse_ReasonPhrase(i64 code);
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpResponse_WriteToH1Conn(Amalgame_Net_Http_HttpResponse* self, i64 conn);
struct _Amalgame_Net_Http_HttpParser {
};
code_string Amalgame_Net_Http_HttpParser_Crlf();
code_string Amalgame_Net_Http_HttpParser_DoubleCrlf();
Amalgame_Net_Http_HttpRequest* Amalgame_Net_Http_HttpParser_ParseHeaders(code_string headerBlock);
Amalgame_Net_Http_HttpRequest* Amalgame_Net_Http_HttpParser_ParseFull(code_string raw);
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpParser_ParseResponse(code_string raw);
struct _Amalgame_Net_Http_HttpServer {
};
code_string Amalgame_Net_Http_HttpServer_BadRequestResponse();
code_string Amalgame_Net_Http_HttpServer_InternalErrorResponse();
code_string Amalgame_Net_Http_HttpServer_NotFoundResponse();
struct _Amalgame_Net_Http_HttpRequestBuilder {
    code_string Method;
    code_string Url;
    AmalgameMap* Headers;
    code_string Body;
};
struct _Amalgame_Net_Http_ParsedUrl {
    code_string Scheme;
    code_string Host;
    i64 Port;
    code_string Path;
};
struct _Amalgame_Net_Http_HttpClient {
};
Amalgame_Net_Http_HttpRequestBuilder* Amalgame_Net_Http_HttpRequestBuilder_new();
Amalgame_Net_Http_HttpRequestBuilder* Amalgame_Net_Http_HttpRequestBuilder_Header(Amalgame_Net_Http_HttpRequestBuilder* self, code_string name, code_string value);
Amalgame_Net_Http_HttpRequestBuilder* Amalgame_Net_Http_HttpRequestBuilder_BodyText(Amalgame_Net_Http_HttpRequestBuilder* self, code_string s);
Amalgame_Net_Http_HttpRequestBuilder* Amalgame_Net_Http_HttpRequestBuilder_BodyJson(Amalgame_Net_Http_HttpRequestBuilder* self, code_string jsonText);
Amalgame_Net_Http_HttpRequestBuilder* Amalgame_Net_Http_HttpRequestBuilder_BodyForm(Amalgame_Net_Http_HttpRequestBuilder* self, AmalgameMap* form);
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpRequestBuilder_Send(Amalgame_Net_Http_HttpRequestBuilder* self);
Amalgame_Net_Http_ParsedUrl* Amalgame_Net_Http_ParsedUrl_new();
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpClient_Get(code_string url);
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpClient_Post(code_string url, code_string body, code_string contentType);
Amalgame_Net_Http_HttpRequestBuilder* Amalgame_Net_Http_HttpClient_Request(code_string method, code_string url);
Amalgame_Net_Http_HttpResponse* Amalgame_Net_Http_HttpClient_Execute(Amalgame_Net_Http_HttpRequestBuilder* b);
Amalgame_Net_Http_ParsedUrl* Amalgame_Net_Http_HttpClient_ParseUrl(code_string url);
struct _Amalgame_Web_Route {
    code_string Method;
    code_string Path;
    AmalgameClosure* Handler;
    AmalgameList* Segments;
    AmalgameList* ParamNames;
};
struct _Amalgame_Web_RouteMatch {
    Amalgame_Web_Route* Route;
    AmalgameMap* Params;
};
struct _Amalgame_Web_Router {
    AmalgameList* Routes;
};
Amalgame_Web_Route* Amalgame_Web_Route_new(code_string method, code_string path, AmalgameClosure* handler);
AmalgameList* Amalgame_Web_Route_SplitPath(code_string path);
Amalgame_Web_RouteMatch* Amalgame_Web_RouteMatch_new(Amalgame_Web_Route* r);
Amalgame_Web_Router* Amalgame_Web_Router_new();
Amalgame_Web_Router* Amalgame_Web_Router_Add(Amalgame_Web_Router* self, code_string method, code_string path, AmalgameClosure* handler);
Amalgame_Web_Router* Amalgame_Web_Router_Get(Amalgame_Web_Router* self, code_string path, AmalgameClosure* handler);
Amalgame_Web_Router* Amalgame_Web_Router_Post(Amalgame_Web_Router* self, code_string path, AmalgameClosure* handler);
Amalgame_Web_Router* Amalgame_Web_Router_Put(Amalgame_Web_Router* self, code_string path, AmalgameClosure* handler);
Amalgame_Web_Router* Amalgame_Web_Router_Patch(Amalgame_Web_Router* self, code_string path, AmalgameClosure* handler);
Amalgame_Web_Router* Amalgame_Web_Router_Delete(Amalgame_Web_Router* self, code_string path, AmalgameClosure* handler);
Amalgame_Web_RouteMatch* Amalgame_Web_Router_Match(Amalgame_Web_Router* self, code_string method, code_string path);
Amalgame_Web_RouteMatch* Amalgame_Web_Router_TryMatchOne(Amalgame_Web_Route* r, AmalgameList* reqSegs);
struct _Amalgame_Web_Session {
    code_string Id;
    AmalgameMap* Data;
    i64 CreatedAt;
    i64 TouchedAt;
    i64 MaxAgeSec;
    AmalgameMutex* Mu;
};
struct _Amalgame_Web_MemorySessionStore {
    AmalgameMap* Sessions;
    code_string CookieName;
    code_bool CookieSecure;
    code_string CookieSameSite;
    code_string CookiePath;
    i64 CookieMaxAge;
    AmalgameMutex* Mu;
};
Amalgame_Web_Session* Amalgame_Web_Session_new(code_string id);
code_string Amalgame_Web_Session_Get(Amalgame_Web_Session* self, code_string key);
void Amalgame_Web_Session_Set(Amalgame_Web_Session* self, code_string key, code_string value);
void Amalgame_Web_Session_Delete(Amalgame_Web_Session* self, code_string key);
void Amalgame_Web_Session_Clear(Amalgame_Web_Session* self);
code_bool Amalgame_Web_Session_Has(Amalgame_Web_Session* self, code_string key);
Amalgame_Web_MemorySessionStore* Amalgame_Web_MemorySessionStore_new();
Amalgame_Web_MemorySessionStore* Amalgame_Web_MemorySessionStore_WithCookieName(Amalgame_Web_MemorySessionStore* self, code_string n);
Amalgame_Web_MemorySessionStore* Amalgame_Web_MemorySessionStore_WithCookieSecure(Amalgame_Web_MemorySessionStore* self, code_bool s);
Amalgame_Web_MemorySessionStore* Amalgame_Web_MemorySessionStore_WithCookieSameSite(Amalgame_Web_MemorySessionStore* self, code_string ss);
Amalgame_Web_MemorySessionStore* Amalgame_Web_MemorySessionStore_WithCookiePath(Amalgame_Web_MemorySessionStore* self, code_string p);
Amalgame_Web_MemorySessionStore* Amalgame_Web_MemorySessionStore_WithCookieMaxAge(Amalgame_Web_MemorySessionStore* self, i64 a);
Amalgame_Net_Http_Cookie* Amalgame_Web_MemorySessionStore_MakeCookie(Amalgame_Web_MemorySessionStore* self, code_string sessionId);
Amalgame_Web_MemorySessionStore* Amalgame_Web_MemorySessionStore_FromMap(AmalgameMap* cfg);
Amalgame_Web_Session* Amalgame_Web_MemorySessionStore_Find(Amalgame_Web_MemorySessionStore* self, code_string id);
Amalgame_Web_Session* Amalgame_Web_MemorySessionStore_Create(Amalgame_Web_MemorySessionStore* self, code_string id);
void Amalgame_Web_MemorySessionStore_Destroy(Amalgame_Web_MemorySessionStore* self, code_string id);
i64 Amalgame_Web_MemorySessionStore_Size(Amalgame_Web_MemorySessionStore* self);
struct _Amalgame_Web_WebContext {
    AmalgameMap* Params;
    AmalgameMap* State;
    Amalgame_Web_Session* Session;
    code_string RequestId;
};
Amalgame_Web_WebContext* Amalgame_Web_WebContext_new();
code_string Amalgame_Web_WebContext_Param(Amalgame_Web_WebContext* self, code_string name);
code_string Amalgame_Web_WebContext_StateOf(Amalgame_Web_WebContext* self, code_string name);
struct _Amalgame_Web_SecurityHeaders {
    code_string Hsts;
    code_string ContentSecurityPolicy;
    code_string FrameOptions;
    code_bool ContentTypeOptions;
    code_string ReferrerPolicy;
    code_string PermissionsPolicy;
    code_string Coop;
    code_string Coep;
};
Amalgame_Web_SecurityHeaders* Amalgame_Web_SecurityHeaders_new();
Amalgame_Web_SecurityHeaders* Amalgame_Web_SecurityHeaders_StrictHtml();
Amalgame_Web_SecurityHeaders* Amalgame_Web_SecurityHeaders_StrictApi();
Amalgame_Web_SecurityHeaders* Amalgame_Web_SecurityHeaders_WithHsts(Amalgame_Web_SecurityHeaders* self, i64 maxAgeSec, code_bool includeSubdomains, code_bool preload);
Amalgame_Web_SecurityHeaders* Amalgame_Web_SecurityHeaders_WithCsp(Amalgame_Web_SecurityHeaders* self, code_string policy);
Amalgame_Web_SecurityHeaders* Amalgame_Web_SecurityHeaders_WithoutCsp(Amalgame_Web_SecurityHeaders* self);
Amalgame_Web_SecurityHeaders* Amalgame_Web_SecurityHeaders_WithFrameOptions(Amalgame_Web_SecurityHeaders* self, code_string value);
Amalgame_Web_SecurityHeaders* Amalgame_Web_SecurityHeaders_WithContentTypeOptions(Amalgame_Web_SecurityHeaders* self, code_bool on);
Amalgame_Web_SecurityHeaders* Amalgame_Web_SecurityHeaders_WithReferrerPolicy(Amalgame_Web_SecurityHeaders* self, code_string value);
Amalgame_Web_SecurityHeaders* Amalgame_Web_SecurityHeaders_WithPermissionsPolicy(Amalgame_Web_SecurityHeaders* self, code_string value);
Amalgame_Web_SecurityHeaders* Amalgame_Web_SecurityHeaders_WithCoop(Amalgame_Web_SecurityHeaders* self, code_string value);
Amalgame_Web_SecurityHeaders* Amalgame_Web_SecurityHeaders_WithCoep(Amalgame_Web_SecurityHeaders* self, code_string value);
Amalgame_Web_SecurityHeaders* Amalgame_Web_SecurityHeaders_FromMap(AmalgameMap* cfg);
Amalgame_Net_Http_HttpResponse* Amalgame_Web_SecurityHeaders_Apply(Amalgame_Web_SecurityHeaders* self, Amalgame_Net_Http_HttpResponse* resp);
struct _Amalgame_Web_Cors {
    AmalgameList* AllowedOrigins;
    AmalgameList* AllowedMethods;
    AmalgameList* AllowedHeaders;
    AmalgameList* ExposedHeaders;
    code_bool AllowCredentials;
    i64 MaxAgeSec;
};
Amalgame_Web_Cors* Amalgame_Web_Cors_new();
Amalgame_Web_Cors* Amalgame_Web_Cors_Disabled();
Amalgame_Web_Cors* Amalgame_Web_Cors_AllowAll();
Amalgame_Web_Cors* Amalgame_Web_Cors_Strict();
Amalgame_Web_Cors* Amalgame_Web_Cors_WithAllowedOrigins(Amalgame_Web_Cors* self, AmalgameList* origins);
Amalgame_Web_Cors* Amalgame_Web_Cors_WithAllowedMethods(Amalgame_Web_Cors* self, AmalgameList* methods);
Amalgame_Web_Cors* Amalgame_Web_Cors_WithAllowedHeaders(Amalgame_Web_Cors* self, AmalgameList* headers);
Amalgame_Web_Cors* Amalgame_Web_Cors_WithExposedHeaders(Amalgame_Web_Cors* self, AmalgameList* headers);
Amalgame_Web_Cors* Amalgame_Web_Cors_WithAllowCredentials(Amalgame_Web_Cors* self, code_bool b);
Amalgame_Web_Cors* Amalgame_Web_Cors_WithMaxAge(Amalgame_Web_Cors* self, i64 seconds);
Amalgame_Web_Cors* Amalgame_Web_Cors_AddOrigin(Amalgame_Web_Cors* self, code_string origin);
Amalgame_Web_Cors* Amalgame_Web_Cors_AddMethod(Amalgame_Web_Cors* self, code_string method);
code_bool Amalgame_Web_Cors_OriginAllowed(Amalgame_Web_Cors* self, code_string origin);
code_string Amalgame_Web_Cors_OriginEcho(Amalgame_Web_Cors* self, code_string origin);
Amalgame_Net_Http_HttpResponse* Amalgame_Web_Cors_Preflight(Amalgame_Web_Cors* self, Amalgame_Net_Http_HttpRequest* req);
Amalgame_Net_Http_HttpResponse* Amalgame_Web_Cors_Apply(Amalgame_Web_Cors* self, Amalgame_Net_Http_HttpRequest* req, Amalgame_Net_Http_HttpResponse* resp);
Amalgame_Web_Cors* Amalgame_Web_Cors_FromMap(AmalgameMap* cfg);
code_string Amalgame_Web_Cors_JoinList(AmalgameList* items, code_string sep);
AmalgameList* Amalgame_Web_Cors_SplitCsv(code_string csv);
struct _Amalgame_Web_RateLimitBucket {
    i64 Count;
    i64 WindowStartNanos;
};
struct _Amalgame_Web_RateLimit {
    i64 MaxRequests;
    i64 WindowNanos;
    code_string KeyStrategy;
    AmalgameMap* Store;
    code_string Backend;
    code_string RedisHost;
    i64 RedisPort;
    code_string RedisKeyPrefix;
    AmalgameRedis* RedisHandle;
    AmalgameMutex* Mu;
};
Amalgame_Web_RateLimitBucket* Amalgame_Web_RateLimitBucket_new();
Amalgame_Web_RateLimit* Amalgame_Web_RateLimit_new();
Amalgame_Web_RateLimit* Amalgame_Web_RateLimit_PerIp(i64 maxRequests, i64 windowSec);
Amalgame_Web_RateLimit* Amalgame_Web_RateLimit_Disabled();
Amalgame_Web_RateLimit* Amalgame_Web_RateLimit_WithMaxRequests(Amalgame_Web_RateLimit* self, i64 n);
Amalgame_Web_RateLimit* Amalgame_Web_RateLimit_WithWindowSec(Amalgame_Web_RateLimit* self, i64 s);
Amalgame_Web_RateLimit* Amalgame_Web_RateLimit_WithKeyStrategy(Amalgame_Web_RateLimit* self, code_string s);
Amalgame_Web_RateLimit* Amalgame_Web_RateLimit_WithBackend(Amalgame_Web_RateLimit* self, code_string b);
Amalgame_Web_RateLimit* Amalgame_Web_RateLimit_WithRedisHost(Amalgame_Web_RateLimit* self, code_string h);
Amalgame_Web_RateLimit* Amalgame_Web_RateLimit_WithRedisPort(Amalgame_Web_RateLimit* self, i64 p);
Amalgame_Web_RateLimit* Amalgame_Web_RateLimit_WithRedisKeyPrefix(Amalgame_Web_RateLimit* self, code_string p);
code_string Amalgame_Web_RateLimit_KeyFor(Amalgame_Web_RateLimit* self, Amalgame_Net_Http_HttpRequest* req);
Amalgame_Net_Http_HttpResponse* Amalgame_Web_RateLimit_Check(Amalgame_Web_RateLimit* self, Amalgame_Net_Http_HttpRequest* req);
Amalgame_Net_Http_HttpResponse* Amalgame_Web_RateLimit_CheckMemory(Amalgame_Web_RateLimit* self, code_string key);
Amalgame_Net_Http_HttpResponse* Amalgame_Web_RateLimit_CheckRedis(Amalgame_Web_RateLimit* self, code_string key);
code_bool Amalgame_Web_RateLimit_EnsureRedisHandle(Amalgame_Web_RateLimit* self);
void Amalgame_Web_RateLimit_Disconnect(Amalgame_Web_RateLimit* self);
Amalgame_Net_Http_HttpResponse* Amalgame_Web_RateLimit_Make429(i64 retryAfterSec);
Amalgame_Web_RateLimit* Amalgame_Web_RateLimit_FromMap(AmalgameMap* cfg);
code_bool Amalgame_Web_RateLimit_Probe(Amalgame_Web_RateLimit* self);
struct _Amalgame_Web_Csrf {
    code_string CookieName;
    code_string HeaderName;
    i64 TokenBytes;
    AmalgameList* SafeMethods;
    AmalgameList* ExemptPaths;
    code_string CookiePath;
    code_bool CookieSecure;
    code_string CookieSameSite;
    i64 CookieMaxAge;
    Amalgame_Random_Random* Rng;
};
Amalgame_Web_Csrf* Amalgame_Web_Csrf_new();
Amalgame_Web_Csrf* Amalgame_Web_Csrf_Default();
Amalgame_Web_Csrf* Amalgame_Web_Csrf_Disabled();
Amalgame_Web_Csrf* Amalgame_Web_Csrf_WithCookieName(Amalgame_Web_Csrf* self, code_string n);
Amalgame_Web_Csrf* Amalgame_Web_Csrf_WithHeaderName(Amalgame_Web_Csrf* self, code_string n);
Amalgame_Web_Csrf* Amalgame_Web_Csrf_WithTokenBytes(Amalgame_Web_Csrf* self, i64 b);
Amalgame_Web_Csrf* Amalgame_Web_Csrf_WithCookiePath(Amalgame_Web_Csrf* self, code_string p);
Amalgame_Web_Csrf* Amalgame_Web_Csrf_WithCookieSecure(Amalgame_Web_Csrf* self, code_bool s);
Amalgame_Web_Csrf* Amalgame_Web_Csrf_WithCookieSameSite(Amalgame_Web_Csrf* self, code_string ss);
Amalgame_Web_Csrf* Amalgame_Web_Csrf_WithCookieMaxAge(Amalgame_Web_Csrf* self, i64 a);
Amalgame_Web_Csrf* Amalgame_Web_Csrf_WithSafeMethods(Amalgame_Web_Csrf* self, AmalgameList* m);
Amalgame_Web_Csrf* Amalgame_Web_Csrf_ExemptPath(Amalgame_Web_Csrf* self, code_string prefix);
Amalgame_Web_Csrf* Amalgame_Web_Csrf_WithRng(Amalgame_Web_Csrf* self, Amalgame_Random_Random* r);
code_string Amalgame_Web_Csrf_HexEncode(AmalgameList* bytes);
code_string Amalgame_Web_Csrf_NewToken(Amalgame_Web_Csrf* self);
code_string Amalgame_Web_Csrf_EnsureToken(Amalgame_Web_Csrf* self, Amalgame_Net_Http_HttpRequest* req, Amalgame_Net_Http_HttpResponse* resp);
Amalgame_Net_Http_HttpResponse* Amalgame_Web_Csrf_Validate(Amalgame_Web_Csrf* self, Amalgame_Net_Http_HttpRequest* req);
code_string Amalgame_Web_Csrf_ReadCookie(Amalgame_Web_Csrf* self, Amalgame_Net_Http_HttpRequest* req);
Amalgame_Net_Http_HttpResponse* Amalgame_Web_Csrf_Deny(Amalgame_Web_Csrf* self, code_string reason);
AmalgameList* Amalgame_Web_Csrf_SplitCsv(code_string csv);
Amalgame_Web_Csrf* Amalgame_Web_Csrf_FromMap(AmalgameMap* cfg);
struct _Amalgame_Web_LogConfig {
    code_string Level;
    code_string File;
    code_bool AccessLog;
    code_string Format;
    code_string RequestIdHeader;
};
Amalgame_Web_LogConfig* Amalgame_Web_LogConfig_new();
Amalgame_Web_LogConfig* Amalgame_Web_LogConfig_WithLevel(Amalgame_Web_LogConfig* self, code_string l);
Amalgame_Web_LogConfig* Amalgame_Web_LogConfig_WithFile(Amalgame_Web_LogConfig* self, code_string p);
Amalgame_Web_LogConfig* Amalgame_Web_LogConfig_WithAccessLog(Amalgame_Web_LogConfig* self, code_bool on);
Amalgame_Web_LogConfig* Amalgame_Web_LogConfig_WithFormat(Amalgame_Web_LogConfig* self, code_string f);
Amalgame_Web_LogConfig* Amalgame_Web_LogConfig_WithRequestIdHeader(Amalgame_Web_LogConfig* self, code_string h);
void Amalgame_Web_LogConfig_Apply(Amalgame_Web_LogConfig* self);
Amalgame_Web_LogConfig* Amalgame_Web_LogConfig_FromMap(AmalgameMap* cfg);
struct _Amalgame_Web_SignedCookieSessionStore {
    code_string Secret;
    code_bool Encrypted;
    code_string CookieName;
    code_bool CookieSecure;
    code_string CookieSameSite;
    code_string CookiePath;
    i64 CookieMaxAge;
};
Amalgame_Web_SignedCookieSessionStore* Amalgame_Web_SignedCookieSessionStore_new(code_string secret);
Amalgame_Web_SignedCookieSessionStore* Amalgame_Web_SignedCookieSessionStore_WithSecret(Amalgame_Web_SignedCookieSessionStore* self, code_string s);
Amalgame_Web_SignedCookieSessionStore* Amalgame_Web_SignedCookieSessionStore_WithEncrypted(Amalgame_Web_SignedCookieSessionStore* self, code_bool e);
Amalgame_Web_SignedCookieSessionStore* Amalgame_Web_SignedCookieSessionStore_WithCookieName(Amalgame_Web_SignedCookieSessionStore* self, code_string n);
Amalgame_Web_SignedCookieSessionStore* Amalgame_Web_SignedCookieSessionStore_WithCookieSecure(Amalgame_Web_SignedCookieSessionStore* self, code_bool s);
Amalgame_Web_SignedCookieSessionStore* Amalgame_Web_SignedCookieSessionStore_WithCookieSameSite(Amalgame_Web_SignedCookieSessionStore* self, code_string ss);
Amalgame_Web_SignedCookieSessionStore* Amalgame_Web_SignedCookieSessionStore_WithCookiePath(Amalgame_Web_SignedCookieSessionStore* self, code_string p);
Amalgame_Web_SignedCookieSessionStore* Amalgame_Web_SignedCookieSessionStore_WithCookieMaxAge(Amalgame_Web_SignedCookieSessionStore* self, i64 a);
code_string Amalgame_Web_SignedCookieSessionStore_Encode(Amalgame_Web_SignedCookieSessionStore* self, Amalgame_Web_Session* s);
Amalgame_Web_Session* Amalgame_Web_SignedCookieSessionStore_Decode(Amalgame_Web_SignedCookieSessionStore* self, code_string cookie_value);
Amalgame_Web_Session* Amalgame_Web_SignedCookieSessionStore_DecodeSigned(Amalgame_Web_SignedCookieSessionStore* self, code_string cookie_value);
code_string Amalgame_Web_SignedCookieSessionStore_EncodeEncrypted(Amalgame_Web_SignedCookieSessionStore* self, code_string payload);
Amalgame_Web_Session* Amalgame_Web_SignedCookieSessionStore_DecodeEncrypted(Amalgame_Web_SignedCookieSessionStore* self, code_string cookie_value);
Amalgame_Net_Http_Cookie* Amalgame_Web_SignedCookieSessionStore_MakeCookie(Amalgame_Web_SignedCookieSessionStore* self, Amalgame_Web_Session* s);
Amalgame_Web_SignedCookieSessionStore* Amalgame_Web_SignedCookieSessionStore_FromMap(AmalgameMap* cfg);
code_string Amalgame_Web_SignedCookieSessionStore_SerializeData(AmalgameMap* d);
AmalgameMap* Amalgame_Web_SignedCookieSessionStore_DeserializeData(code_string raw);
code_bool Amalgame_Web_SignedCookieSessionStore_ConstantTimeEqualStr(code_string a, code_string b);
AmalgameList* Amalgame_Web_SignedCookieSessionStore_DeriveAesKey(code_string secret);
struct _Amalgame_Web_RedisSessionStore {
    code_string Host;
    i64 Port;
    code_string KeyPrefix;
    code_string CookieName;
    code_bool CookieSecure;
    code_string CookieSameSite;
    code_string CookiePath;
    i64 CookieMaxAge;
    i64 MaxAgeSec;
    AmalgameRedis* Handle;
    AmalgameMutex* Mu;
};
Amalgame_Web_RedisSessionStore* Amalgame_Web_RedisSessionStore_new();
Amalgame_Web_RedisSessionStore* Amalgame_Web_RedisSessionStore_WithHost(Amalgame_Web_RedisSessionStore* self, code_string h);
Amalgame_Web_RedisSessionStore* Amalgame_Web_RedisSessionStore_WithPort(Amalgame_Web_RedisSessionStore* self, i64 p);
Amalgame_Web_RedisSessionStore* Amalgame_Web_RedisSessionStore_WithKeyPrefix(Amalgame_Web_RedisSessionStore* self, code_string p);
Amalgame_Web_RedisSessionStore* Amalgame_Web_RedisSessionStore_WithCookieName(Amalgame_Web_RedisSessionStore* self, code_string n);
Amalgame_Web_RedisSessionStore* Amalgame_Web_RedisSessionStore_WithCookieSecure(Amalgame_Web_RedisSessionStore* self, code_bool s);
Amalgame_Web_RedisSessionStore* Amalgame_Web_RedisSessionStore_WithCookieSameSite(Amalgame_Web_RedisSessionStore* self, code_string ss);
Amalgame_Web_RedisSessionStore* Amalgame_Web_RedisSessionStore_WithCookiePath(Amalgame_Web_RedisSessionStore* self, code_string p);
Amalgame_Web_RedisSessionStore* Amalgame_Web_RedisSessionStore_WithCookieMaxAge(Amalgame_Web_RedisSessionStore* self, i64 a);
Amalgame_Web_RedisSessionStore* Amalgame_Web_RedisSessionStore_WithMaxAgeSec(Amalgame_Web_RedisSessionStore* self, i64 a);
code_bool Amalgame_Web_RedisSessionStore_EnsureHandle(Amalgame_Web_RedisSessionStore* self);
void Amalgame_Web_RedisSessionStore_Disconnect(Amalgame_Web_RedisSessionStore* self);
code_bool Amalgame_Web_RedisSessionStore_Probe(Amalgame_Web_RedisSessionStore* self);
Amalgame_Net_Http_Cookie* Amalgame_Web_RedisSessionStore_MakeCookie(Amalgame_Web_RedisSessionStore* self, code_string sessionId);
Amalgame_Web_Session* Amalgame_Web_RedisSessionStore_Create(Amalgame_Web_RedisSessionStore* self, code_string id);
Amalgame_Web_Session* Amalgame_Web_RedisSessionStore_Find(Amalgame_Web_RedisSessionStore* self, code_string id);
code_bool Amalgame_Web_RedisSessionStore_Save(Amalgame_Web_RedisSessionStore* self, Amalgame_Web_Session* s);
void Amalgame_Web_RedisSessionStore_Destroy(Amalgame_Web_RedisSessionStore* self, code_string id);
Amalgame_Web_RedisSessionStore* Amalgame_Web_RedisSessionStore_FromMap(AmalgameMap* cfg);
code_string Amalgame_Web_RedisSessionStore_SerializeData(AmalgameMap* d);
AmalgameMap* Amalgame_Web_RedisSessionStore_DeserializeData(code_string raw);
struct _Amalgame_Web_AcmeConfig {
    code_bool Enabled;
    code_string Domain;
    AmalgameList* Domains;
    code_string Email;
    code_string CertDir;
    code_string AcmeServer;
    code_string CertbotPath;
};
Amalgame_Web_AcmeConfig* Amalgame_Web_AcmeConfig_new();
Amalgame_Web_AcmeConfig* Amalgame_Web_AcmeConfig_WithEnabled(Amalgame_Web_AcmeConfig* self, code_bool e);
Amalgame_Web_AcmeConfig* Amalgame_Web_AcmeConfig_WithEmail(Amalgame_Web_AcmeConfig* self, code_string e);
Amalgame_Web_AcmeConfig* Amalgame_Web_AcmeConfig_WithCertDir(Amalgame_Web_AcmeConfig* self, code_string d);
Amalgame_Web_AcmeConfig* Amalgame_Web_AcmeConfig_WithAcmeServer(Amalgame_Web_AcmeConfig* self, code_string s);
Amalgame_Web_AcmeConfig* Amalgame_Web_AcmeConfig_WithCertbotPath(Amalgame_Web_AcmeConfig* self, code_string p);
Amalgame_Web_AcmeConfig* Amalgame_Web_AcmeConfig_WithDomain(Amalgame_Web_AcmeConfig* self, code_string d);
Amalgame_Web_AcmeConfig* Amalgame_Web_AcmeConfig_WithDomains(Amalgame_Web_AcmeConfig* self, AmalgameList* ds);
code_string Amalgame_Web_AcmeConfig_Validate(Amalgame_Web_AcmeConfig* self);
code_string Amalgame_Web_AcmeConfig_CertPath(Amalgame_Web_AcmeConfig* self);
code_string Amalgame_Web_AcmeConfig_KeyPath(Amalgame_Web_AcmeConfig* self);
code_string Amalgame_Web_AcmeConfig_DomainsCsv(Amalgame_Web_AcmeConfig* self);
Amalgame_Web_AcmeConfig* Amalgame_Web_AcmeConfig_FromMap(AmalgameMap* cfg);
AmalgameList* Amalgame_Web_AcmeConfig_SplitCsv(code_string csv);
struct _Amalgame_Web_TlsBindingConfig {
    code_string MinVersion;
    code_string Alpn;
};
Amalgame_Web_TlsBindingConfig* Amalgame_Web_TlsBindingConfig_new();
Amalgame_Web_TlsBindingConfig* Amalgame_Web_TlsBindingConfig_WithMinVersion(Amalgame_Web_TlsBindingConfig* self, code_string v);
Amalgame_Web_TlsBindingConfig* Amalgame_Web_TlsBindingConfig_WithAlpn(Amalgame_Web_TlsBindingConfig* self, code_string a);
i64 Amalgame_Web_TlsBindingConfig_MinVersionInt(Amalgame_Web_TlsBindingConfig* self);
code_string Amalgame_Web_TlsBindingConfig_Validate(Amalgame_Web_TlsBindingConfig* self);
Amalgame_Web_TlsBindingConfig* Amalgame_Web_TlsBindingConfig_FromMap(AmalgameMap* cfg);
struct _Amalgame_Web_Static {
    code_string Prefix;
    code_string Root;
    i64 CacheMaxAge;
};
Amalgame_Web_Static* Amalgame_Web_Static_new();
Amalgame_Web_Static* Amalgame_Web_Static_New(code_string prefix, code_string dir);
Amalgame_Web_Static* Amalgame_Web_Static_WithCacheMaxAge(Amalgame_Web_Static* self, i64 seconds);
code_bool Amalgame_Web_Static_Matches(Amalgame_Web_Static* self, code_string urlPath);
Amalgame_Net_Http_HttpResponse* Amalgame_Web_Static_Serve(Amalgame_Web_Static* self, Amalgame_Net_Http_HttpRequest* req);
Amalgame_Web_Static* Amalgame_Web_Static_FromMap(AmalgameMap* cfg);
code_string Amalgame_Web_Static_NormalizePrefix(code_string p);
code_string Amalgame_Web_Static_NormalizeRoot(code_string d);
code_string Amalgame_Web_Static_MimeByExtension(code_string ext);
struct _Amalgame_Web_WebApp {
    Amalgame_Web_Router* Routes;
    Amalgame_Web_SecurityHeaders* Headers;
    Amalgame_Web_Cors* CorsCfg;
    Amalgame_Web_Csrf* CsrfCfg;
    Amalgame_Web_RateLimit* Limiter;
    Amalgame_Web_LogConfig* Logging;
    AmalgameList* StaticMounts;
    AmalgameMap* State;
    AmalgameMutex* StateMu;
};
Amalgame_Web_WebApp* Amalgame_Web_WebApp_new();
Amalgame_Web_WebApp* Amalgame_Web_WebApp_New();
Amalgame_Web_WebApp* Amalgame_Web_WebApp_WithSecurityHeaders(Amalgame_Web_WebApp* self, Amalgame_Web_SecurityHeaders* sec);
Amalgame_Web_WebApp* Amalgame_Web_WebApp_WithCors(Amalgame_Web_WebApp* self, Amalgame_Web_Cors* c);
Amalgame_Web_WebApp* Amalgame_Web_WebApp_WithCsrf(Amalgame_Web_WebApp* self, Amalgame_Web_Csrf* c);
Amalgame_Web_WebApp* Amalgame_Web_WebApp_WithRateLimit(Amalgame_Web_WebApp* self, Amalgame_Web_RateLimit* rl);
Amalgame_Web_WebApp* Amalgame_Web_WebApp_WithStatic(Amalgame_Web_WebApp* self, Amalgame_Web_Static* s);
Amalgame_Web_WebApp* Amalgame_Web_WebApp_WithLogging(Amalgame_Web_WebApp* self, Amalgame_Web_LogConfig* lc);
Amalgame_Web_WebApp* Amalgame_Web_WebApp_WithState(Amalgame_Web_WebApp* self, code_string key, code_string value);
Amalgame_Web_WebApp* Amalgame_Web_WebApp_Get(Amalgame_Web_WebApp* self, code_string path, AmalgameClosure* handler);
Amalgame_Web_WebApp* Amalgame_Web_WebApp_Post(Amalgame_Web_WebApp* self, code_string path, AmalgameClosure* handler);
Amalgame_Web_WebApp* Amalgame_Web_WebApp_Put(Amalgame_Web_WebApp* self, code_string path, AmalgameClosure* handler);
Amalgame_Web_WebApp* Amalgame_Web_WebApp_Patch(Amalgame_Web_WebApp* self, code_string path, AmalgameClosure* handler);
Amalgame_Web_WebApp* Amalgame_Web_WebApp_Delete(Amalgame_Web_WebApp* self, code_string path, AmalgameClosure* handler);
Amalgame_Net_Http_HttpResponse* Amalgame_Web_WebApp_Handle(Amalgame_Web_WebApp* self, Amalgame_Net_Http_HttpRequest* req);
Amalgame_Net_Http_HttpResponse* Amalgame_Web_WebApp_applyResponseMiddlewares(Amalgame_Web_WebApp* self, Amalgame_Net_Http_HttpRequest* req, Amalgame_Net_Http_HttpResponse* resp);
i64 Amalgame_Web_WebApp_Serve(Amalgame_Web_WebApp* self, i64 port);
i64 Amalgame_Web_WebApp_ServeMt(Amalgame_Web_WebApp* self, i64 port);
i64 Amalgame_Web_WebApp_ServeWith(Amalgame_Web_WebApp* self, i64 port, AmalgameNetHttpServerConfig* cfg);
i64 Amalgame_Web_WebApp_ServeMtWith(Amalgame_Web_WebApp* self, i64 port, AmalgameNetHttpServerConfig* cfg);
i64 Amalgame_Web_WebApp_ServeAsync(Amalgame_Web_WebApp* self, i64 port);
i64 Amalgame_Web_WebApp_ServeAsyncWith(Amalgame_Web_WebApp* self, i64 port, AmalgameNetHttpServerConfig* cfg);
struct _Amalgame_Crypto_Sha256 {
};
struct _Amalgame_Crypto_Hmac {
};
struct _Amalgame_Crypto_Bytes {
};
struct _Amalgame_Crypto_AeadResult {
    code_bool Ok;
    AmalgameList* Plaintext;
};
struct _Amalgame_Crypto_Aead {
};
struct _Amalgame_Crypto_JwsKey {
    i64 Handle;
    code_string Algo;
};
Amalgame_Crypto_Sha256* Amalgame_Crypto_Sha256_new();
AmalgameList* Amalgame_Crypto_Sha256_Bytes(AmalgameList* data);
code_string Amalgame_Crypto_Sha256_Hex(AmalgameList* data);
code_string Amalgame_Crypto_Sha256_OfString(code_string s);
Amalgame_Crypto_Hmac* Amalgame_Crypto_Hmac_new();
AmalgameList* Amalgame_Crypto_Hmac_Sha256(AmalgameList* key, AmalgameList* msg);
code_string Amalgame_Crypto_Hmac_Sha256Hex(AmalgameList* key, AmalgameList* msg);
code_string Amalgame_Crypto_Hmac_Sha256OfStrings(code_string key, code_string msg);
Amalgame_Crypto_Bytes* Amalgame_Crypto_Bytes_new();
AmalgameList* Amalgame_Crypto_Bytes_FromString(code_string s);
code_string Amalgame_Crypto_Bytes_ToString(AmalgameList* b);
code_string Amalgame_Crypto_Bytes_ToHex(AmalgameList* b);
code_string Amalgame_Crypto_Bytes_ToBase64Url(AmalgameList* b);
AmalgameList* Amalgame_Crypto_Bytes_FromBase64Url(code_string s);
AmalgameList* Amalgame_Crypto_Bytes_FromHex(code_string h);
Amalgame_Crypto_AeadResult* Amalgame_Crypto_AeadResult_new();
Amalgame_Crypto_Aead* Amalgame_Crypto_Aead_new();
AmalgameList* Amalgame_Crypto_Aead_SealAes256Gcm(AmalgameList* key, AmalgameList* nonce, AmalgameList* aad, AmalgameList* plaintext);
Amalgame_Crypto_AeadResult* Amalgame_Crypto_Aead_OpenAes256Gcm(AmalgameList* key, AmalgameList* nonce, AmalgameList* aad, AmalgameList* ciphertext_tag);
AmalgameList* Amalgame_Crypto_Aead_SealAes256GcmAuto(AmalgameList* key, AmalgameList* aad, AmalgameList* plaintext);
Amalgame_Crypto_AeadResult* Amalgame_Crypto_Aead_OpenAes256GcmAuto(AmalgameList* key, AmalgameList* aad, AmalgameList* bundle);
Amalgame_Crypto_JwsKey* Amalgame_Crypto_JwsKey_new();
Amalgame_Crypto_JwsKey* Amalgame_Crypto_JwsKey_GenerateEs256();
Amalgame_Crypto_JwsKey* Amalgame_Crypto_JwsKey_GenerateRs256();
Amalgame_Crypto_JwsKey* Amalgame_Crypto_JwsKey_FromPemPrivate(code_string pem);
code_string Amalgame_Crypto_JwsKey_ToPemPrivate(Amalgame_Crypto_JwsKey* self);
code_string Amalgame_Crypto_JwsKey_Algorithm(Amalgame_Crypto_JwsKey* self);
code_bool Amalgame_Crypto_JwsKey_IsValid(Amalgame_Crypto_JwsKey* self);
code_string Amalgame_Crypto_JwsKey_JwkPublic(Amalgame_Crypto_JwsKey* self);
code_string Amalgame_Crypto_JwsKey_ThumbprintBase64Url(Amalgame_Crypto_JwsKey* self);
AmalgameList* Amalgame_Crypto_JwsKey_Sign(Amalgame_Crypto_JwsKey* self, AmalgameList* msg);
code_bool Amalgame_Crypto_JwsKey_Verify(Amalgame_Crypto_JwsKey* self, AmalgameList* msg, AmalgameList* sig);
struct _Amalgame_DateTime_DateTime {
};
struct _Amalgame_DateTime_DateTimeUtil {
};
struct _Amalgame_DateTime_Duration {
    i64 nanos;
};
struct _Amalgame_DateTime_InstantResult {
    code_bool Ok;
    Amalgame_DateTime_Instant* Value;
    code_string Error;
};
struct _Amalgame_DateTime_Instant {
    i64 nanos;
};
struct _Amalgame_DateTime_Stopwatch {
    i64 startNanos;
};
i64 Amalgame_DateTime_DateTime_NowNanos();
i64 Amalgame_DateTime_DateTime_NowMonotonicNanos();
i64 Amalgame_DateTime_DateTime_ParseErrorSentinel();
code_bool Amalgame_DateTime_DateTime_IsParseError(i64 nanos);
code_bool Amalgame_DateTime_DateTimeUtil_IsDigit(code_string c);
i64 Amalgame_DateTime_DateTimeUtil_DigitVal(code_string c);
code_string Amalgame_DateTime_DateTimeUtil_PadZero(code_string s, i64 width);
AmalgameList* Amalgame_DateTime_DateTimeUtil_CivilFromDays(i64 days);
i64 Amalgame_DateTime_DateTimeUtil_DaysFromCivil(i64 year, i64 month, i64 day);
AmalgameList* Amalgame_DateTime_DateTimeUtil_Breakdown(i64 nanos);
code_string Amalgame_DateTime_DateTimeUtil_FormatIso(i64 nanos);
i64 Amalgame_DateTime_DateTimeUtil_ParseIsoNanos(code_string s);
Amalgame_DateTime_Duration* Amalgame_DateTime_Duration_new(i64 nanos);
Amalgame_DateTime_Duration* Amalgame_DateTime_Duration_FromSeconds(i64 s);
Amalgame_DateTime_Duration* Amalgame_DateTime_Duration_FromMillis(i64 ms);
Amalgame_DateTime_Duration* Amalgame_DateTime_Duration_FromMinutes(i64 m);
Amalgame_DateTime_Duration* Amalgame_DateTime_Duration_FromHours(i64 h);
Amalgame_DateTime_Duration* Amalgame_DateTime_Duration_FromDays(i64 d);
i64 Amalgame_DateTime_Duration_Nanos(Amalgame_DateTime_Duration* self);
i64 Amalgame_DateTime_Duration_Millis(Amalgame_DateTime_Duration* self);
i64 Amalgame_DateTime_Duration_Seconds(Amalgame_DateTime_Duration* self);
double Amalgame_DateTime_Duration_SecondsFloat(Amalgame_DateTime_Duration* self);
Amalgame_DateTime_Duration* Amalgame_DateTime_Duration_Plus(Amalgame_DateTime_Duration* self, Amalgame_DateTime_Duration* other);
Amalgame_DateTime_Duration* Amalgame_DateTime_Duration_Minus(Amalgame_DateTime_Duration* self, Amalgame_DateTime_Duration* other);
Amalgame_DateTime_Duration* Amalgame_DateTime_Duration_Times(Amalgame_DateTime_Duration* self, i64 k);
Amalgame_DateTime_Duration* Amalgame_DateTime_Duration_Negate(Amalgame_DateTime_Duration* self);
code_bool Amalgame_DateTime_Duration_IsZero(Amalgame_DateTime_Duration* self);
code_bool Amalgame_DateTime_Duration_IsNegative(Amalgame_DateTime_Duration* self);
code_string Amalgame_DateTime_Duration_Format(Amalgame_DateTime_Duration* self);
Amalgame_DateTime_InstantResult* Amalgame_DateTime_InstantResult_new();
code_bool Amalgame_DateTime_InstantResult_IsOk(Amalgame_DateTime_InstantResult* self);
Amalgame_DateTime_Instant* Amalgame_DateTime_InstantResult_GetValue(Amalgame_DateTime_InstantResult* self);
code_string Amalgame_DateTime_InstantResult_GetError(Amalgame_DateTime_InstantResult* self);
Amalgame_DateTime_Instant* Amalgame_DateTime_Instant_new(i64 nanos);
Amalgame_DateTime_Instant* Amalgame_DateTime_Instant_Now();
Amalgame_DateTime_Instant* Amalgame_DateTime_Instant_FromUnixSeconds(i64 s);
Amalgame_DateTime_Instant* Amalgame_DateTime_Instant_FromUnixMillis(i64 ms);
Amalgame_DateTime_Instant* Amalgame_DateTime_Instant_FromUnixNanos(i64 ns);
Amalgame_DateTime_InstantResult* Amalgame_DateTime_Instant_Parse(code_string s);
i64 Amalgame_DateTime_Instant_UnixNanos(Amalgame_DateTime_Instant* self);
i64 Amalgame_DateTime_Instant_UnixMillis(Amalgame_DateTime_Instant* self);
i64 Amalgame_DateTime_Instant_UnixSeconds(Amalgame_DateTime_Instant* self);
code_string Amalgame_DateTime_Instant_Format(Amalgame_DateTime_Instant* self);
i64 Amalgame_DateTime_Instant_Year(Amalgame_DateTime_Instant* self);
i64 Amalgame_DateTime_Instant_Month(Amalgame_DateTime_Instant* self);
i64 Amalgame_DateTime_Instant_Day(Amalgame_DateTime_Instant* self);
i64 Amalgame_DateTime_Instant_Hour(Amalgame_DateTime_Instant* self);
i64 Amalgame_DateTime_Instant_Minute(Amalgame_DateTime_Instant* self);
i64 Amalgame_DateTime_Instant_Second(Amalgame_DateTime_Instant* self);
Amalgame_DateTime_Instant* Amalgame_DateTime_Instant_Add(Amalgame_DateTime_Instant* self, Amalgame_DateTime_Duration* d);
Amalgame_DateTime_Instant* Amalgame_DateTime_Instant_Subtract(Amalgame_DateTime_Instant* self, Amalgame_DateTime_Duration* d);
Amalgame_DateTime_Duration* Amalgame_DateTime_Instant_Since(Amalgame_DateTime_Instant* self, Amalgame_DateTime_Instant* other);
code_bool Amalgame_DateTime_Instant_IsBefore(Amalgame_DateTime_Instant* self, Amalgame_DateTime_Instant* other);
code_bool Amalgame_DateTime_Instant_IsAfter(Amalgame_DateTime_Instant* self, Amalgame_DateTime_Instant* other);
code_bool Amalgame_DateTime_Instant_Equals(Amalgame_DateTime_Instant* self, Amalgame_DateTime_Instant* other);
Amalgame_DateTime_Stopwatch* Amalgame_DateTime_Stopwatch_new();
Amalgame_DateTime_Duration* Amalgame_DateTime_Stopwatch_Elapsed(Amalgame_DateTime_Stopwatch* self);
Amalgame_DateTime_Duration* Amalgame_DateTime_Stopwatch_Reset(Amalgame_DateTime_Stopwatch* self);
struct _Amalgame_Random_Random {
    i64 state;
    i64 inc;
};
Amalgame_Random_Random* Amalgame_Random_Random_new(i64 seed);
Amalgame_Random_Random* Amalgame_Random_Random_FromSystem();
AmalgameList* Amalgame_Random_Random_SystemBytes(i64 n);
i64 Amalgame_Random_Random_NextUInt32(Amalgame_Random_Random* self);
i64 Amalgame_Random_Random_NextInt(Amalgame_Random_Random* self);
i64 Amalgame_Random_Random_IntRange(Amalgame_Random_Random* self, i64 min, i64 max);
double Amalgame_Random_Random_Float(Amalgame_Random_Random* self);
code_bool Amalgame_Random_Random_Bool(Amalgame_Random_Random* self);
AmalgameList* Amalgame_Random_Random_Bytes(Amalgame_Random_Random* self, i64 n);
struct _Amalgame_Logging_Log {
};
Amalgame_Logging_Log* Amalgame_Logging_Log_new();
void Amalgame_Logging_Log_SetMinLevel(code_string name);
code_string Amalgame_Logging_Log_GetMinLevel();
void Amalgame_Logging_Log_SetFile(code_string path);
code_string Amalgame_Logging_Log_GetFile();
void Amalgame_Logging_Log_Debug(code_string msg);
void Amalgame_Logging_Log_Info(code_string msg);
void Amalgame_Logging_Log_Warn(code_string msg);
void Amalgame_Logging_Log_Error(code_string msg);
struct _Amalgame_Formats_Json_JsonValue {
    Amalgame_Formats_Json_JsonKind Kind;
    code_bool B;
    i64 I;
    double F;
    code_string S;
    AmalgameList* Items;
    AmalgameList* ObjKeys;
    AmalgameList* ObjVals;
};
struct _Amalgame_Formats_Json_JsonError {
    code_string Message;
    i64 Line;
    i64 Column;
};
struct _Amalgame_Formats_Json_JsonResult {
    code_bool Ok;
    Amalgame_Formats_Json_JsonValue* Value;
    Amalgame_Formats_Json_JsonError* Error;
};
struct _Amalgame_Formats_Json_JsonParser {
    code_string Source;
    i64 Pos;
    i64 Line;
    i64 Column;
    code_bool Failed;
    code_string ErrMsg;
    i64 ErrLine;
    i64 ErrCol;
};
struct _Amalgame_Formats_Json_Json {
};
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_JsonValue_new();
code_bool Amalgame_Formats_Json_JsonValue_IsNull(Amalgame_Formats_Json_JsonValue* self);
code_bool Amalgame_Formats_Json_JsonValue_IsBool(Amalgame_Formats_Json_JsonValue* self);
code_bool Amalgame_Formats_Json_JsonValue_IsInt(Amalgame_Formats_Json_JsonValue* self);
code_bool Amalgame_Formats_Json_JsonValue_IsFloat(Amalgame_Formats_Json_JsonValue* self);
code_bool Amalgame_Formats_Json_JsonValue_IsNumber(Amalgame_Formats_Json_JsonValue* self);
code_bool Amalgame_Formats_Json_JsonValue_IsString(Amalgame_Formats_Json_JsonValue* self);
code_bool Amalgame_Formats_Json_JsonValue_IsArray(Amalgame_Formats_Json_JsonValue* self);
code_bool Amalgame_Formats_Json_JsonValue_IsObject(Amalgame_Formats_Json_JsonValue* self);
code_bool Amalgame_Formats_Json_JsonValue_AsBool(Amalgame_Formats_Json_JsonValue* self);
i64 Amalgame_Formats_Json_JsonValue_AsInt(Amalgame_Formats_Json_JsonValue* self);
double Amalgame_Formats_Json_JsonValue_AsFloat(Amalgame_Formats_Json_JsonValue* self);
code_string Amalgame_Formats_Json_JsonValue_AsString(Amalgame_Formats_Json_JsonValue* self);
AmalgameList* Amalgame_Formats_Json_JsonValue_AsArray(Amalgame_Formats_Json_JsonValue* self);
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_JsonValue_Get(Amalgame_Formats_Json_JsonValue* self, code_string key);
code_bool Amalgame_Formats_Json_JsonValue_Has(Amalgame_Formats_Json_JsonValue* self, code_string key);
AmalgameList* Amalgame_Formats_Json_JsonValue_Keys(Amalgame_Formats_Json_JsonValue* self);
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_JsonValue_At(Amalgame_Formats_Json_JsonValue* self, i64 i);
i64 Amalgame_Formats_Json_JsonValue_Length(Amalgame_Formats_Json_JsonValue* self);
void Amalgame_Formats_Json_JsonValue_SetNull(Amalgame_Formats_Json_JsonValue* self);
void Amalgame_Formats_Json_JsonValue_SetBool(Amalgame_Formats_Json_JsonValue* self, code_bool b);
void Amalgame_Formats_Json_JsonValue_SetInt(Amalgame_Formats_Json_JsonValue* self, i64 n);
void Amalgame_Formats_Json_JsonValue_SetFloat(Amalgame_Formats_Json_JsonValue* self, double f);
void Amalgame_Formats_Json_JsonValue_SetString(Amalgame_Formats_Json_JsonValue* self, code_string s);
void Amalgame_Formats_Json_JsonValue_SetArray(Amalgame_Formats_Json_JsonValue* self, AmalgameList* xs);
void Amalgame_Formats_Json_JsonValue_SetObject(Amalgame_Formats_Json_JsonValue* self, AmalgameList* keys, AmalgameList* vals);
void Amalgame_Formats_Json_JsonValue_AppendItem(Amalgame_Formats_Json_JsonValue* self, Amalgame_Formats_Json_JsonValue* v);
void Amalgame_Formats_Json_JsonValue_AppendEntry(Amalgame_Formats_Json_JsonValue* self, code_string key, Amalgame_Formats_Json_JsonValue* v);
Amalgame_Formats_Json_JsonError* Amalgame_Formats_Json_JsonError_new(code_string msg, i64 line, i64 col);
Amalgame_Formats_Json_JsonResult* Amalgame_Formats_Json_JsonResult_new();
Amalgame_Formats_Json_JsonParser* Amalgame_Formats_Json_JsonParser_new(code_string source);
code_bool Amalgame_Formats_Json_JsonParser_HasFailed(Amalgame_Formats_Json_JsonParser* self);
code_string Amalgame_Formats_Json_JsonParser_ErrorMsg(Amalgame_Formats_Json_JsonParser* self);
i64 Amalgame_Formats_Json_JsonParser_ErrorLine(Amalgame_Formats_Json_JsonParser* self);
i64 Amalgame_Formats_Json_JsonParser_ErrorCol(Amalgame_Formats_Json_JsonParser* self);
void Amalgame_Formats_Json_JsonParser_Fail(Amalgame_Formats_Json_JsonParser* self, code_string msg);
code_bool Amalgame_Formats_Json_JsonParser_AtEnd(Amalgame_Formats_Json_JsonParser* self);
code_string Amalgame_Formats_Json_JsonParser_Peek(Amalgame_Formats_Json_JsonParser* self);
code_string Amalgame_Formats_Json_JsonParser_PeekAt(Amalgame_Formats_Json_JsonParser* self, i64 offset);
code_string Amalgame_Formats_Json_JsonParser_Advance(Amalgame_Formats_Json_JsonParser* self);
void Amalgame_Formats_Json_JsonParser_SkipWs(Amalgame_Formats_Json_JsonParser* self);
code_bool Amalgame_Formats_Json_JsonParser_MatchLit(Amalgame_Formats_Json_JsonParser* self, code_string lit);
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_JsonParser_ParseTopLevel(Amalgame_Formats_Json_JsonParser* self);
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_JsonParser_ParseValue(Amalgame_Formats_Json_JsonParser* self);
code_bool Amalgame_Formats_Json_JsonParser_IsDigit(code_string c);
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_JsonParser_ParseObject(Amalgame_Formats_Json_JsonParser* self);
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_JsonParser_ParseArray(Amalgame_Formats_Json_JsonParser* self);
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_JsonParser_ParseString(Amalgame_Formats_Json_JsonParser* self);
i64 Amalgame_Formats_Json_JsonParser_ParseHex4(Amalgame_Formats_Json_JsonParser* self);
i64 Amalgame_Formats_Json_JsonParser_HexDigit(code_string c);
code_string Amalgame_Formats_Json_JsonParser_Bs();
code_string Amalgame_Formats_Json_JsonParser_Ff();
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_JsonParser_ParseNumber(Amalgame_Formats_Json_JsonParser* self);
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_JsonParser_ParseBool(Amalgame_Formats_Json_JsonParser* self);
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_JsonParser_ParseNull(Amalgame_Formats_Json_JsonParser* self);
Amalgame_Formats_Json_Json* Amalgame_Formats_Json_Json_new();
Amalgame_Formats_Json_JsonResult* Amalgame_Formats_Json_Json_Parse(code_string source);
code_string Amalgame_Formats_Json_Json_Encode(Amalgame_Formats_Json_JsonValue* v);
code_string Amalgame_Formats_Json_Json_EncodeArray(Amalgame_Formats_Json_JsonValue* v);
code_string Amalgame_Formats_Json_Json_EncodeObject(Amalgame_Formats_Json_JsonValue* v);
code_string Amalgame_Formats_Json_Json_EscapeString(code_string s);
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_Json_NullValue();
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_Json_OfBool(code_bool b);
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_Json_OfInt(i64 n);
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_Json_OfFloat(double f);
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_Json_OfString(code_string s);
Amalgame_Formats_Json_JsonValue* Amalgame_Formats_Json_Json_OfArray(AmalgameList* xs);

App_Program* App_Program_new();
void App_Program_Main(code_string* args);
App_About_Page* App_About_Page_new();
App_Api_Info_Page* App_Api_Info_Page_new();
App_Index_Page* App_Index_Page_new();
App_Users_Id_Page* App_Users_Id_Page_new();
App_Routes* App_Routes_new();
Amalgame_Net_Http_HttpResponse* App_About_Page_GET(Amalgame_Web_WebContext* ctx);
Amalgame_Net_Http_HttpResponse* App_Api_Info_Page_GET(Amalgame_Web_WebContext* ctx);
Amalgame_Net_Http_HttpResponse* App_Api_Info_Page_POST(Amalgame_Web_WebContext* ctx);
Amalgame_Net_Http_HttpResponse* App_Index_Page_GET(Amalgame_Web_WebContext* ctx);
Amalgame_Net_Http_HttpResponse* App_Users_Id_Page_GET(Amalgame_Web_WebContext* ctx);
void App_Routes_Register(Amalgame_Web_Router* r);
Amalgame_Web_WebApp* App_Routes_Bind(Amalgame_Web_WebApp* app);
typedef struct LamEnv_0 {
    char _empty;
} LamEnv_0;
static void* lam_0_fn(void* __envRaw, void* __arg0);
typedef struct LamEnv_1 {
    char _empty;
} LamEnv_1;
static void* lam_1_fn(void* __envRaw, void* __arg0);
typedef struct LamEnv_2 {
    char _empty;
} LamEnv_2;
static void* lam_2_fn(void* __envRaw, void* __arg0);
typedef struct LamEnv_3 {
    char _empty;
} LamEnv_3;
static void* lam_3_fn(void* __envRaw, void* __arg0);
typedef struct LamEnv_4 {
    char _empty;
} LamEnv_4;
static void* lam_4_fn(void* __envRaw, void* __arg0);
typedef struct LamEnv_5 {
    char _empty;
} LamEnv_5;
static void* lam_5_fn(void* __envRaw, void* __arg0);
typedef struct LamEnv_6 {
    char _empty;
} LamEnv_6;
static void* lam_6_fn(void* __envRaw, void* __arg0);
typedef struct LamEnv_7 {
    char _empty;
} LamEnv_7;
static void* lam_7_fn(void* __envRaw, void* __arg0);
typedef struct LamEnv_8 {
    char _empty;
} LamEnv_8;
static void* lam_8_fn(void* __envRaw, void* __arg0);
typedef struct LamEnv_9 {
    char _empty;
} LamEnv_9;
static void* lam_9_fn(void* __envRaw, void* __arg0);
struct _App_Program {
};

void App_Program_Main(code_string* args);

App_Program* App_Program_new() {
    App_Program* self = (App_Program*) GC_MALLOC(sizeof(App_Program));
    return self;
}

void App_Program_Main(code_string* args) {
    #line 38 "server.am"
    Amalgame_Web_WebApp* app = Amalgame_Web_WebApp_New();
    #line 39 "server.am"
    App_Routes_Bind(app);
    #line 41 "server.am"
    Console_WriteLine("Mosaic v0.6 — filesystem routing + public/ demo");
    #line 47 "server.am"
    Amalgame_Web_Router* router = app->Routes;
    #line 48 "server.am"
    Console_WriteLine(code_string_concat((code_string_concat("  ", String_FromInt(AmalgameList_count(router->Routes)))), " routes registered from app/"));
    #line 49 "server.am"
    Console_WriteLine(code_string_concat("  static mount(s): ", String_FromInt(AmalgameList_count(app->StaticMounts))));
    #line 50 "server.am"
    Console_WriteLine("  open http://localhost:3000/ in any browser");
    #line 51 "server.am"
    Console_WriteLine("");
    #line 53 "server.am"
    Amalgame_Web_WebApp_Serve(app, 3000);
}

struct _App_About_Page {
};

Amalgame_Net_Http_HttpResponse* App_About_Page_GET(Amalgame_Web_WebContext* ctx);

App_About_Page* App_About_Page_new() {
    App_About_Page* self = (App_About_Page*) GC_MALLOC(sizeof(App_About_Page));
    return self;
}

Amalgame_Net_Http_HttpResponse* App_About_Page_GET(Amalgame_Web_WebContext* ctx) {
    #line 16 "_routes.am"
    return Amalgame_Net_Http_HttpResponse_Text(Amalgame_Net_Http_HttpResponse_New(), code_string_concat((code_string_concat((code_string_concat("About this demo:\n", "  - 4 static .am files under app/\n")), "  - one shell script generates _routes.am\n")), "  - amalgame-web's Router does the matching\n"));
}

struct _App_Api_Info_Page {
};

Amalgame_Net_Http_HttpResponse* App_Api_Info_Page_GET(Amalgame_Web_WebContext* ctx);
Amalgame_Net_Http_HttpResponse* App_Api_Info_Page_POST(Amalgame_Web_WebContext* ctx);

App_Api_Info_Page* App_Api_Info_Page_new() {
    App_Api_Info_Page* self = (App_Api_Info_Page*) GC_MALLOC(sizeof(App_Api_Info_Page));
    return self;
}

Amalgame_Net_Http_HttpResponse* App_Api_Info_Page_GET(Amalgame_Web_WebContext* ctx) {
    #line 30 "_routes.am"
    code_string json = code_string_concat("{\"app\":\"mosaic-fs-demo\",\"version\":\"0.2.0\",", "\"routing\":\"filesystem-discovered\"}");
    #line 32 "_routes.am"
    return Amalgame_Net_Http_HttpResponse_Json(Amalgame_Net_Http_HttpResponse_New(), json);
}

Amalgame_Net_Http_HttpResponse* App_Api_Info_Page_POST(Amalgame_Web_WebContext* ctx) {
    #line 36 "_routes.am"
    return Amalgame_Net_Http_HttpResponse_Json(Amalgame_Net_Http_HttpResponse_Status(Amalgame_Net_Http_HttpResponse_New(), 201), "{\"created\":true}");
}

struct _App_Index_Page {
};

Amalgame_Net_Http_HttpResponse* App_Index_Page_GET(Amalgame_Web_WebContext* ctx);

App_Index_Page* App_Index_Page_new() {
    App_Index_Page* self = (App_Index_Page*) GC_MALLOC(sizeof(App_Index_Page));
    return self;
}

Amalgame_Net_Http_HttpResponse* App_Index_Page_GET(Amalgame_Web_WebContext* ctx) {
    #line 51 "_routes.am"
    code_string livereload = code_string_concat("<script>const w=new WebSocket('ws://localhost:35729/');", "w.onmessage=e=>e.data==='reload'?location.reload():null;</script>");
    #line 53 "_routes.am"
    code_string html = code_string_concat((code_string_concat((code_string_concat((code_string_concat((code_string_concat((code_string_concat((code_string_concat((code_string_concat((code_string_concat((code_string_concat((code_string_concat((code_string_concat((code_string_concat((code_string_concat((code_string_concat("<!doctype html><html><head><title>Mosaic v0.3</title>", "<style>body{font-family:system-ui,sans-serif;max-width:42em;margin:3em auto;color:#222}")), "code{background:#f4f4f4;padding:2px 6px;border-radius:3px}")), "</style></head><body>")), "<h1>Mosaic filesystem routing</h1>")), "<p>Routes discovered from <code>app/</code> by ")), "<code>tools/mosaic-routes.sh</code>:</p>")), "<ul>")), "<li><code>/</code> — this page</li>")), "<li><code>/about</code> — static</li>")), "<li><code>/users/:id</code> — dynamic param</li>")), "<li><code>/api/info</code> — JSON, multi-verb</li>")), "</ul>")), "<p>Edit me and watch the page reload automatically (mosaic dev v0.3+).</p>")), livereload)), "</body></html>");
    #line 69 "_routes.am"
    return Amalgame_Net_Http_HttpResponse_Html(Amalgame_Net_Http_HttpResponse_New(), html);
}

struct _App_Users_Id_Page {
};

Amalgame_Net_Http_HttpResponse* App_Users_Id_Page_GET(Amalgame_Web_WebContext* ctx);

App_Users_Id_Page* App_Users_Id_Page_new() {
    App_Users_Id_Page* self = (App_Users_Id_Page*) GC_MALLOC(sizeof(App_Users_Id_Page));
    return self;
}

Amalgame_Net_Http_HttpResponse* App_Users_Id_Page_GET(Amalgame_Web_WebContext* ctx) {
    #line 79 "_routes.am"
    code_string id = Amalgame_Web_WebContext_Param(ctx, "id");
    #line 80 "_routes.am"
    return Amalgame_Net_Http_HttpResponse_Text(Amalgame_Net_Http_HttpResponse_New(), code_string_concat((code_string_concat("User #", id)), "\n"));
}

struct _App_Routes {
};

void App_Routes_Register(Amalgame_Web_Router* r);
Amalgame_Web_WebApp* App_Routes_Bind(Amalgame_Web_WebApp* app);

App_Routes* App_Routes_new() {
    App_Routes* self = (App_Routes*) GC_MALLOC(sizeof(App_Routes));
    return self;
}

void App_Routes_Register(Amalgame_Web_Router* r) {
    #line 90 "_routes.am"
    Amalgame_Web_Router_Add(r, "GET", "/about", ({ LamEnv_0* __env_0 = (LamEnv_0*)code_alloc(sizeof(LamEnv_0)); AmalgameClosure_new((void*)lam_0_fn, __env_0); }));
    #line 92 "_routes.am"
    Amalgame_Web_Router_Add(r, "GET", "/api/info", ({ LamEnv_1* __env_1 = (LamEnv_1*)code_alloc(sizeof(LamEnv_1)); AmalgameClosure_new((void*)lam_1_fn, __env_1); }));
    #line 93 "_routes.am"
    Amalgame_Web_Router_Add(r, "POST", "/api/info", ({ LamEnv_2* __env_2 = (LamEnv_2*)code_alloc(sizeof(LamEnv_2)); AmalgameClosure_new((void*)lam_2_fn, __env_2); }));
    #line 95 "_routes.am"
    Amalgame_Web_Router_Add(r, "GET", "/", ({ LamEnv_3* __env_3 = (LamEnv_3*)code_alloc(sizeof(LamEnv_3)); AmalgameClosure_new((void*)lam_3_fn, __env_3); }));
    #line 97 "_routes.am"
    Amalgame_Web_Router_Add(r, "GET", "/users/:id", ({ LamEnv_4* __env_4 = (LamEnv_4*)code_alloc(sizeof(LamEnv_4)); AmalgameClosure_new((void*)lam_4_fn, __env_4); }));
}

Amalgame_Web_WebApp* App_Routes_Bind(Amalgame_Web_WebApp* app) {
    #line 105 "_routes.am"
    Amalgame_Web_WebApp_Get(app, "/about", ({ LamEnv_5* __env_5 = (LamEnv_5*)code_alloc(sizeof(LamEnv_5)); AmalgameClosure_new((void*)lam_5_fn, __env_5); }));
    #line 107 "_routes.am"
    Amalgame_Web_WebApp_Get(app, "/api/info", ({ LamEnv_6* __env_6 = (LamEnv_6*)code_alloc(sizeof(LamEnv_6)); AmalgameClosure_new((void*)lam_6_fn, __env_6); }));
    #line 108 "_routes.am"
    Amalgame_Web_WebApp_Post(app, "/api/info", ({ LamEnv_7* __env_7 = (LamEnv_7*)code_alloc(sizeof(LamEnv_7)); AmalgameClosure_new((void*)lam_7_fn, __env_7); }));
    #line 110 "_routes.am"
    Amalgame_Web_WebApp_Get(app, "/", ({ LamEnv_8* __env_8 = (LamEnv_8*)code_alloc(sizeof(LamEnv_8)); AmalgameClosure_new((void*)lam_8_fn, __env_8); }));
    #line 112 "_routes.am"
    Amalgame_Web_WebApp_Get(app, "/users/:id", ({ LamEnv_9* __env_9 = (LamEnv_9*)code_alloc(sizeof(LamEnv_9)); AmalgameClosure_new((void*)lam_9_fn, __env_9); }));
    #line 115 "_routes.am"
    Amalgame_Web_WebApp_WithStatic(app, Amalgame_Web_Static_New("/", "./public"));
    #line 116 "_routes.am"
    return app;
}

static void* lam_0_fn(void* __envRaw, void* __arg0) {
    LamEnv_0* __env = (LamEnv_0*)__envRaw;
    Amalgame_Web_WebContext* ctx = (Amalgame_Web_WebContext*)__arg0;
    return (void*)(intptr_t)(App_About_Page_GET(ctx));
}
static void* lam_1_fn(void* __envRaw, void* __arg0) {
    LamEnv_1* __env = (LamEnv_1*)__envRaw;
    Amalgame_Web_WebContext* ctx = (Amalgame_Web_WebContext*)__arg0;
    return (void*)(intptr_t)(App_Api_Info_Page_GET(ctx));
}
static void* lam_2_fn(void* __envRaw, void* __arg0) {
    LamEnv_2* __env = (LamEnv_2*)__envRaw;
    Amalgame_Web_WebContext* ctx = (Amalgame_Web_WebContext*)__arg0;
    return (void*)(intptr_t)(App_Api_Info_Page_POST(ctx));
}
static void* lam_3_fn(void* __envRaw, void* __arg0) {
    LamEnv_3* __env = (LamEnv_3*)__envRaw;
    Amalgame_Web_WebContext* ctx = (Amalgame_Web_WebContext*)__arg0;
    return (void*)(intptr_t)(App_Index_Page_GET(ctx));
}
static void* lam_4_fn(void* __envRaw, void* __arg0) {
    LamEnv_4* __env = (LamEnv_4*)__envRaw;
    Amalgame_Web_WebContext* ctx = (Amalgame_Web_WebContext*)__arg0;
    return (void*)(intptr_t)(App_Users_Id_Page_GET(ctx));
}
static void* lam_5_fn(void* __envRaw, void* __arg0) {
    LamEnv_5* __env = (LamEnv_5*)__envRaw;
    Amalgame_Web_WebContext* ctx = (Amalgame_Web_WebContext*)__arg0;
    return (void*)(intptr_t)(App_About_Page_GET(ctx));
}
static void* lam_6_fn(void* __envRaw, void* __arg0) {
    LamEnv_6* __env = (LamEnv_6*)__envRaw;
    Amalgame_Web_WebContext* ctx = (Amalgame_Web_WebContext*)__arg0;
    return (void*)(intptr_t)(App_Api_Info_Page_GET(ctx));
}
static void* lam_7_fn(void* __envRaw, void* __arg0) {
    LamEnv_7* __env = (LamEnv_7*)__envRaw;
    Amalgame_Web_WebContext* ctx = (Amalgame_Web_WebContext*)__arg0;
    return (void*)(intptr_t)(App_Api_Info_Page_POST(ctx));
}
static void* lam_8_fn(void* __envRaw, void* __arg0) {
    LamEnv_8* __env = (LamEnv_8*)__envRaw;
    Amalgame_Web_WebContext* ctx = (Amalgame_Web_WebContext*)__arg0;
    return (void*)(intptr_t)(App_Index_Page_GET(ctx));
}
static void* lam_9_fn(void* __envRaw, void* __arg0) {
    LamEnv_9* __env = (LamEnv_9*)__envRaw;
    Amalgame_Web_WebContext* ctx = (Amalgame_Web_WebContext*)__arg0;
    return (void*)(intptr_t)(App_Users_Id_Page_GET(ctx));
}

int main(int argc, char** argv) {
    GC_INIT();
    code_runtime_init_args(argc, argv);
    App_Program_Main((code_string*)argv);
    return code_exit_code;
}
