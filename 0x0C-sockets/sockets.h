#ifndef _SOCKETS_H_
#define _SOCKETS_H_

#include <stdlib.h>

#define true 1
#define false 0

/**
 * struct todo_s - struct representing a todo item
 * @id: todo id number
 * @title: title of todo item
 * @description: description of todo item
 * @repr: string representation of todo item
 * @repr_len: length of string representation of todo item
 */
typedef struct todo_s
{
	int    id;
	char  *title;
	char  *description;
	char  *repr;
	size_t repr_len;
} todo_t;

/**
 * enum http_method_e - enumeration of HTTP method types
 * @GET:     GET method
 * @HEAD:    HEAD method
 * @POST:    POST method
 * @PUT:     PUT method
 * @DELETE:  DELETE method
 * @CONNECT: CONNECT method
 * @OPTIONS: OPTIONS method
 * @TRACE:   TRACE method
 * @UNKNOWN: unknown HTTP method
 **/
typedef enum http_method_e
{
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	CONNECT,
	OPTIONS,
	TRACE,
	UNKNOWN
} http_method_t;

#define NUM_HTTP_METHODS 8

/**
 * enum http_header_field_e - enumerates all standard HTTP headers
 * @A_IM: A_IM
 * @Accept: Accept
 * @Accept_CH: Accept_CH
 * @Accept_Charset: Accept_Charset
 * @Accept_Datetime: Accept_Datetime
 * @Accept_Encoding: Accept_Encoding
 * @Accept_Language: Accept_Language
 * @Accept_Patch: Accept_Patch
 * @Accept_Ranges: Accept_Ranges
 * @Access_Control_Allow_Origin: Access_Control_Allow_Origin
 * @Access_Control_Allow_Credentials: Access_Control_Allow_Credentials
 * @Access_Control_Expose_Headers: Access_Control_Expose_Headers
 * @Access_Control_Max_Age: Access_Control_Max_Age
 * @Access_Control_Allow_Methods: Access_Control_Allow_Methods
 * @Access_Control_Allow_Headers: Access_Control_Allow_Headers
 * @Access_Control_Request_Method: Access_Control_Request_Method
 * @Access_Control_Request_Headers: Access_Control_Request_Headers
 * @Age: Age
 * @Allow: Allow
 * @Alt_Svc: Alt_Svc
 * @Authorization: Authorization
 * @Cache_Control: Cache_Control
 * @Cache_Control: Cache_Control
 * @Connection: Connection
 * @Connection: Connection
 * @Content_Disposition: Content_Disposition
 * @Content_Encoding: Content_Encoding
 * @Content_Encoding: Content_Encoding
 * @Content_Language: Content_Language
 * @Content_Length: Content_Length
 * @Content_Length: Content_Length
 * @Content_Location: Content_Location
 * @Content_MD5: Content_MD5
 * @Content_MD5: Content_MD5
 * @Content_Range: Content_Range
 * @Content_Type: Content_Type
 * @Content_Type: Content_Type
 * @Cookie: Cookie
 * @Date: Date
 * @Date: Date
 * @Delta_Base: Delta_Base
 * @ETag: ETag
 * @Expect: Expect
 * @Expires: Expires
 * @Field name: Field name
 * @Forwarded: Forwarded
 * @From: From
 * @HTTP2_Settings: HTTP2_Settings
 * @Host: Host
 * @IM: IM
 * @If_Match: If_Match
 * @If_Modified_Since: If_Modified_Since
 * @If_None_Match: If_None_Match
 * @If_Range: If_Range
 * @If_Unmodified_Since: If_Unmodified_Since
 * @Last_Modified: Last_Modified
 * @Link: Link
 * @Location: Location
 * @Max_Forwards: Max_Forwards
 * @Origin: Origin
 * @P3P: P3P
 * @Pragma: Pragma
 * @Pragma: Pragma
 * @Prefer: Prefer
 * @Preference_Applied: Preference_Applied
 * @Proxy_Authenticate: Proxy_Authenticate
 * @Proxy_Authorization: Proxy_Authorization
 * @Public_Key_Pins: Public_Key_Pins
 * @Range: Range
 * @Referer: Referer
 * @Retry_After: Retry_After
 * @Server: Server
 * @Set_Cookie: Set_Cookie
 * @Strict_Transport_Security: Strict_Transport_Security
 * @TE: TE
 * @Tk: Tk
 * @Trailer: Trailer
 * @Trailer: Trailer
 * @Transfer_Encoding: Transfer_Encoding
 * @Transfer_Encoding: Transfer_Encoding
 * @Upgrade: Upgrade
 * @Upgrade: Upgrade
 * @User_Agent: User_Agent
 * @Vary: Vary
 * @Via: Via
 * @Via: Via
 * @WWW_Authenticate: WWW_Authenticate
 * @Warning: Warning
 * @X_Frame_Options: X_Frame_Options
 **/
typedef enum http_header_field_e
{
	A_IM,
	Accept,
	Accept_CH,
	Accept_Charset,
	Accept_Datetime,
	Accept_Encoding,
	Accept_Language,
	Accept_Patch,
	Accept_Ranges,
	Access_Control_Allow_Origin,
	Access_Control_Allow_Credentials,
	Access_Control_Expose_Headers,
	Access_Control_Max_Age,
	Access_Control_Allow_Methods,
	Access_Control_Allow_Headers,
	Access_Control_Request_Method,
	Access_Control_Request_Headers,
	Age,
	Allow,
	Alt_Svc,
	Authorization,
	Cache_Control,
	Connection,
	Content_Disposition,
	Content_Encoding,
	Content_Language,
	Content_Length,
	Content_Location,
	Content_MD5,
	Content_Range,
	Content_Type,
	Cookie,
	Date,
	Delta_Base,
	ETag,
	Expect,
	Expires,
	Forwarded,
	From,
	HTTP2_Settings,
	Host,
	IM,
	If_Match,
	If_Modified_Since,
	If_None_Match,
	If_Range,
	If_Unmodified_Since,
	Last_Modified,
	Link,
	Location,
	Max_Forwards,
	Origin,
	P3P,
	Pragma,
	Prefer,
	Preference_Applied,
	Proxy_Authenticate,
	Proxy_Authorization,
	Public_Key_Pins,
	Range,
	Referer,
	Retry_After,
	Server,
	Set_Cookie,
	Strict_Transport_Security,
	TE,
	Tk,
	Trailer,
	Transfer_Encoding,
	Upgrade,
	User_Agent,
	Vary,
	Via,
	WWW_Authenticate,
	Warning,
	X_Frame_Options
} http_header_field_t;

/**
 * struct http_param_s - HTTP body parameter struct
 * @key: parameter key
 * @value: parameter value
 * @next: pointer to next HTTP body parameter struct
 */
typedef struct http_param_s
{
	char *key;
	char *value;
	struct http_param_s *next;
} http_param_t;

/**
 * struct http_header_s - HTTP header struct
 * @field: HTTP header field enum
 * @value: pointer to value of field
 * @next: pointer to next HTTP header struct
 */
typedef struct http_header_s
{
	char *field;
	char *value;
	struct http_header_s *next;
} http_header_t;

/**
 * struct http_request_s - struct describing an HTTP request
 * @raw_request: raw request
 * @method: HTTP method
 * @method_str: pointer to HTTP method string from original HTTP request
 * @uri: request URI
 * @version: HTTP version (string)
 * @headers: linked list of http_header_t structs
 * @body: pointer to HTTP request body
 * @query_params: linked list of http_param_t structs passed via query
 * @body_params: linked list of http_param_t structs passed via body
 */
typedef struct http_request_s
{
	char          *raw_request;
	http_method_t  method;
	char          *method_str;
	char          *uri;
	char          *version;
	http_header_t *headers;
	char          *body;
	http_param_t  *query_params;
	http_param_t  *body_params;
} http_request_t;

void   take_requests(int sockid);
void   print_path_and_queries(char *buffer);
void   print_headers(char *buffer);
void   print_body_params(char *buffer);
int    eval_request(char *buffer, int sockid, int client_id);
char  *get_param(http_param_t *params, char *key);
void   add_todo(todo_t *todos, int id, char *title, char *description);
char  *make_repr(int id, char *title, char *description);
int    post(char *body, int id, int client_id, int sockid, todo_t *todos);



#endif /* _SOCKETS_H_ */
