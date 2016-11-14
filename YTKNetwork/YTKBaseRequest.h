//
//  YTKBaseRequest.h
//
//  Copyright (c) 2012-2016 YTKNetwork https://github.com/yuantiku
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#import <Foundation/Foundation.h>

// 下面假定属性和方法参数返回值的为 nonnull 类型
NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT NSString *const YTKRequestValidationErrorDomain;

NS_ENUM(NSInteger) {
    YTKRequestValidationErrorInvalidStatusCode = -8,
    YTKRequestValidationErrorInvalidJSONFormat = -9,
};

///  HTTP Request method.
///  HTTP 请求的方法
typedef NS_ENUM(NSInteger, YTKRequestMethod) {
    YTKRequestMethodGET = 0,
    YTKRequestMethodPOST,
    YTKRequestMethodHEAD,
    YTKRequestMethodPUT,
    YTKRequestMethodDELETE,
    YTKRequestMethodPATCH,
};

///  Request serializer type.
typedef NS_ENUM(NSInteger, YTKRequestSerializerType) {
    YTKRequestSerializerTypeHTTP = 0,
    YTKRequestSerializerTypeJSON,
};

///  Response serializer type, which determines response serialization process and
///  the type of `responseObject`.
typedef NS_ENUM(NSInteger, YTKResponseSerializerType) {
    /// NSData type
    YTKResponseSerializerTypeHTTP,
    /// JSON object type
    YTKResponseSerializerTypeJSON,
    /// NSXMLParser type
    YTKResponseSerializerTypeXMLParser,
};

///  Request priority
typedef NS_ENUM(NSInteger, YTKRequestPriority) {
    YTKRequestPriorityLow = -4L,
    YTKRequestPriorityDefault = 0,
    YTKRequestPriorityHigh = 4,
};

///  声明
@protocol AFMultipartFormData;
@class YTKBaseRequest;
    
typedef void (^AFConstructingBlock)(id<AFMultipartFormData> formData);
typedef void (^AFURLSessionTaskProgressBlock)(NSProgress *);

typedef void (^YTKRequestCompletionBlock)(__kindof YTKBaseRequest *request);

///  The YTKRequestDelegate protocol defines several optional methods you can use
///  to receive network-related messages. All the delegate methods will be called
///  on the main queue.
///  YTKRequestDelegate 协议定义了几个可选的方法，可以通过它们来接受网络相关的信息。
///  所有的代理方法会在主线程中调用
@protocol YTKRequestDelegate <NSObject>

@optional
///  Tell the delegate that the request has finished successfully.
///
///  @param request The corresponding request.
///  通知代理请求成功的结束
///  request ：相应的请求
- (void)requestFinished:(__kindof YTKBaseRequest *)request;

///  Tell the delegate that the request has failed.
///
///  @param request The corresponding request.
///  通知代理请求失败
///  request ：相应的请求
- (void)requestFailed:(__kindof YTKBaseRequest *)request;

@end

///  The YTKRequestAccessory protocol defines several optional methods that can be
///  used to track the status of a request. Objects that conforms this protocol
///  ("accessories") can perform additional configurations accordingly. All the
///  accessory methods will be called on the main queue.
///  YTKRequestAccessory 协议定义了几个可选的方法，可以用来追踪请求的状态。符合该协议的类可以添加相应
///  额外的配置。所有的附件方法都是在主线程中调用的
@protocol YTKRequestAccessory <NSObject>

@optional

///  Inform the accessory that the request is about to start.
///
///  @param request The corresponding request.
///  通知附件请求即将开始
///  request ： 相应的请求
- (void)requestWillStart:(id)request;

///  Inform the accessory that the request is about to stop. This method is called
///  before executing `requestFinished` and `successCompletionBlock`.
///
///  @param request The corresponding request.
///  通知附件请求即将结束，这个方法在执行'requestFinished' 和 'successCompletionBlock' 之前
- (void)requestWillStop:(id)request;

///  Inform the accessory that the request has already stoped. This method is called
///  after executing `requestFinished` and `successCompletionBlock`.
///
///  @param request The corresponding request.
///  通知附件请求已经结束。这个方法在调用‘requestFinished’和'successCompletionBlock'之后
- (void)requestDidStop:(id)request;

@end

///  YTKBaseRequest is the abstract class of network request. It provides many options
///  for constructing request. It's the base class of `YTKRequest`.
///  YTKBaseRequest 是网络请求的抽象类。它提供了很多选项来构造请求。他是 'YTKRequest' 的基类。
@interface YTKBaseRequest : NSObject

#pragma mark - Request and Response Information
///=============================================================================
/// @name Request and Response Information
///=============================================================================

///  The underlying NSURLSessionTask.
///
///  @warning This value is actually nil and should not be accessed before the request starts.
///  潜在的 NSURLSessionTask
///  注意：这个值实际上为 nil，并且在请求开始之前不应该被访问
@property (nonatomic, strong, readonly) NSURLSessionTask *requestTask;

///  Shortcut for `requestTask.currentRequest`.
///  访问 requestTask.currentRequest 的捷径
@property (nonatomic, strong, readonly) NSURLRequest *currentRequest;

///  Shortcut for `requestTask.originalRequest`.
///  访问 requestTask.originalRequest 的捷径
@property (nonatomic, strong, readonly) NSURLRequest *originalRequest;

///  Shortcut for `requestTask.response`.
///  访问 requestTask.reqponse 的捷径
@property (nonatomic, strong, readonly) NSHTTPURLResponse *response;

///  The response status code.
///  响应的状态码
@property (nonatomic, readonly) NSInteger responseStatusCode;

///  The response header fields.
///  响应的头部
@property (nonatomic, strong, readonly, nullable) NSDictionary *responseHeaders;

///  The raw data representation of response. Note this value can be nil if request failed.
///  响应的原始数据 data 表示。注意在请求失败的时候这个值可能为 nil
@property (nonatomic, strong, readonly, nullable) NSData *responseData;

///  The string representation of response. Note this value can be nil if request failed.
///  响应的原始数据 string 表示。注意在请求失败的时候这个值可能为 nil
@property (nonatomic, strong, readonly, nullable) NSString *responseString;

///  This serialized response object. The actual type of this object is determined by
///  `YTKResponseSerializerType`. Note this value can be nil if request failed.
///
///  @discussion If `resumableDownloadPath` and DownloadTask is using, this value will
///              be the path to which file is successfully saved (NSURL).
///  序列化的响应类。这个类的真实类型由 YTKResponseSerializerType 决定。注意在请求失败的时候这个值可能为 nil
///
///  讨论：如果使用 ‘resumableDownloadPath’ 的下载请求，这个值将会是成功保存的文件路径。
@property (nonatomic, strong, readonly, nullable) id responseObject;

///  If you use `YTKResponseSerializerTypeJSON`, this is a convenience (and sematic) getter
///  for the response object. Otherwise this value is nil.
///  如果使用 YTKResponseSerializerTypeJSON,这是一个访问响应对象的一个便捷的方法。否则这个值将为 nil
@property (nonatomic, strong, readonly, nullable) id responseJSONObject;

///  This error can be either serialization error or network error. If nothing wrong happens
///  this value will be nil.
@property (nonatomic, strong, readonly, nullable) NSError *error;

///  Return cancelled state of request task.
@property (nonatomic, readonly, getter=isCancelled) BOOL cancelled;

///  Executing state of request task.
///  执行任务的的状态
@property (nonatomic, readonly, getter=isExecuting) BOOL executing;


#pragma mark - Request Configuration
///=============================================================================
/// @name Request Configuration
///=============================================================================

///  Tag can be used to identify request. Default value is 0.
///  Tag 可以识别请求。默认值是 0
@property (nonatomic) NSInteger tag;

///  The userInfo can be used to store additional info about the request. Default is nil.
///  userInfo 可以用来存储额外的请求信息，默认值为 nil
@property (nonatomic, strong, nullable) NSDictionary *userInfo;

///  The delegate object of the request. If you choose block style callback you can ignore this.
///  Default is nil.
/// 请求的代理类。如果你使用 block 方式回掉，可以忽略掉。默认值为 nil
@property (nonatomic, weak, nullable) id<YTKRequestDelegate> delegate;

///  The success callback. Note if this value is not nil and `requestFinished` delegate method is
///  also implemented, both will be executed but delegate method is first called. This block
///  will be called on the main queue.
///  成功的回掉。注意如果这个值不为 nil 并且 'requestFinished‘ 代理方法同样被实现，两个都会被执行，并且代理方法会先被调用。这个 block 将会被在主线程中被调用。
@property (nonatomic, copy, nullable) YTKRequestCompletionBlock successCompletionBlock;

///  The failure callback. Note if this value is not nil and `requestFailed` delegate method is
///  also implemented, both will be executed but delegate method is first called. This block
///  will be called on the main queue.
/// 失败的回掉。注意如果这个值不为 nil 并且 requestFailed 代理方法同样被实现，两个都会被执行，但是代理方法会先被执行。这个 block 会在主线程中被调用。
@property (nonatomic, copy, nullable) YTKRequestCompletionBlock failureCompletionBlock;

///  This can be used to add several accossories object. Note if you use `addAccessory` to add acceesory
///  this array will be automatically created. Default is nil.
///  这个可以用来添加几个附件类。注意如果你用 addAccessroy 来添加附件，这个数组将会自动创建。默认值为nil
@property (nonatomic, strong, nullable) NSMutableArray<id<YTKRequestAccessory>> *requestAccessories;

///  This can be use to construct HTTP body when needed in POST request. Default is nil.
///  在 POST 请求中，在必要时可以用来构建 HTTP 的 body。默认值为 nil
@property (nonatomic, copy, nullable) AFConstructingBlock constructingBodyBlock;

///  This value is used to perform resumable download request. Default is nil.
///
///  @discussion NSURLSessionDownloadTask is used when this value is not nil. If request succeed, file will
///              be saved to this path automatically. For this to work, server must support `Range` and response
///              with proper `If-Modified-Since` and/or `Etag`. See `NSURLSessionDownloadTask` for more detail.
///  这个用于执行可恢复的下载请求，默认值为 nil
///
///  描述：NSURLSessionDownloadTask 在这个值不为 nil 时被使用。如果请求成功，文件将会自动的保存在这个地址。
///  服务器必须支持 ‘Range’ 并且相应信息中需要包含合适的 `If-Modified-Since` 和／或 `Etag`
///

///  (1)IMS（If-Modified-Since）
///  IMS 是标准的 HTTP 请求头标签，在发送 HTTP 请求时，把浏览器缓存页面的最后修改时间一起发到服务器去，服务器会把这个时间与服务器上实际文件的最后修改时间进行比较。
///  Last-Modified 和 If-Modified-Since 配套使用。
///  如果时间一致，那么返回 HTTP 状态 304（不返回文件内容），客户端使用本地缓存显示。
///  如果时间不一致，那么返回 HTTP 状态 200 和新的文件内容，客户端接收到之后，会丢弃旧文件，把新文件缓存起来，使用新文件显示
///  (2)ETag
///  一种不透明的标识符，由 web 服务器根据 URL 上的资源的特定版本而指定。
///  如果那个 URL 上的资源内容发生改变，一个新的不一样的 ETag 就会被分配，可以快速的比较，以确定两个版本的资源是否相同。
///  ETag 的比较只对同一个 URL 有意义，不同 URL 的资源的 ETag 值可能相同，也可能不同。
///  “ETag”和 “If-None-Match” 配套使用。
///  客户端请求之后，服务器可能会比较客户端的ETag和当前版本资源的ETag。如果ETag值匹配，这就意味着资源没有改变，服务器便会发送回一个极短的响应，包含HTTP “304 未修改”的状态。304状态告诉客户端，它的缓存版本是最新的，并应该使用它。
/*
 HTTP 206状态
 206 状态码表示：客户端通过发送范围请求头 Range 抓取到了资源的部分数据。
 请求一个资源的时候，服务器可能会返回以下两个头部信息：
 Accept-Ranges:bytes  该相应头表明服务器支持 Range 请求，以及服务器支持的单位是字节（这也是唯一可用的单位）。我们还可以知道：服务器支持断点续传，以及支持同时下载文件的多个部分，也就是说下载工具可以利用范围请求加速下载该文件。 Accept-Range:none 相应头表示服务器不支持范围请求。
 Content-Length:3603 表明相应实体的大小，（单位：字节）
 
 如何发送 range 的请求头
 在请求头中加入
 Range：bytes=0-1024
 如果指定的偏移量是有效的，服务器会返回一个 HTTP 206 状态码，如果偏移量是无效的，则服务器会返回一个 HTTP 416 状态码（请求范围无法满足）
 */
@property (nonatomic, strong, nullable) NSString *resumableDownloadPath;

///  You can use this block to track the download progress. See also `resumableDownloadPath`.
///  你可以使用这个 block 来追踪下载过程。
@property (nonatomic, copy, nullable) AFURLSessionTaskProgressBlock resumableDownloadProgressBlock;

///  The priority of the request. Effective only on iOS 8+. Default is `YTKRequestPriorityDefault`.
///  请求的优先级。只有在 iOS 8+ 上有效。默认值 YTKRequestPriorityDefault
@property (nonatomic) YTKRequestPriority requestPriority;

///  Set completion callbacks
- (void)setCompletionBlockWithSuccess:(nullable YTKRequestCompletionBlock)success
                              failure:(nullable YTKRequestCompletionBlock)failure;

///  Nil out both success and failure callback blocks.
- (void)clearCompletionBlock;

///  Convenience method to add request accessory. See also `requestAccessories`.
///  便利的添加请求附件。
- (void)addAccessory:(id<YTKRequestAccessory>)accessory;


#pragma mark - Request Action
///=============================================================================
/// @name Request Action
///=============================================================================

///  Append self to request queue and start the request.
///  追加到请求队列并开始请求
- (void)start;

///  Remove self from request queue and cancel the request.
///  从请求队列中移除，并取消请求
- (void)stop;

///  Convenience method to start the request with block callbacks.
///  同时开始请求并设置回掉的方法
- (void)startWithCompletionBlockWithSuccess:(nullable YTKRequestCompletionBlock)success
                                    failure:(nullable YTKRequestCompletionBlock)failure;


#pragma mark - Subclass Override
///=============================================================================
/// @name Subclass Override
///=============================================================================

///  Called on background thread after request succeded but before switching to main thread. Note if
///  cache is loaded, this method WILL be called on the main thread, just like `requestCompleteFilter`.
/// 在请求成功之后，但是在切换到主线程之前调用。
/// 注意：如果加载缓存，这个方法将会在主线程上调用，就像 requestCompleteFilter
- (void)requestCompletePreprocessor;

///  Called on the main thread after request succeeded.
///  在请求成功之后在主线程上调用
- (void)requestCompleteFilter;

///  Called on background thread after request failed but before switching to main thread. See also
///  `requestCompletePreprocessor`.
///  在请求失败之后，但是在切换到主线程之前调用
- (void)requestFailedPreprocessor;

///  Called on the main thread when request failed.
///  请求失败时在主线程调用
- (void)requestFailedFilter;

///  The baseURL of request. This should only contain the host part of URL, e.g., http://www.example.com.
///  See also `requestUrl`
/// 请求的 baseURL. 这个应该只包含 URL 的主站
- (NSString *)baseUrl;

///  The URL path of request. This should only contain the path part of URL, e.g., /v1/user. See alse `baseUrl`.
///
///  @discussion This will be concated with `baseUrl` using [NSURL URLWithString:relativeToURL].
///              Because of this, it is recommended that the usage should stick to rules stated above.
///              Otherwise the result URL may not be correctly formed. See also `URLString:relativeToURL`
///              for more information.
///
///              Additionaly, if `requestUrl` itself is a valid URL, it will be used as the result URL and
///              `baseUrl` will be ignored.
///  请求的 URL，这个应该只包含 URL 的路径的一部分。
///  描述：这个值将会使用 [NSURL URLWithString:relativeToURL] 链接在一起。
///  因此，使用时建议遵循上述规则。否则，组合的 URL 可能是错误的。
///  另外，如果 requestUrl 自身是一个有效的 URL，它将会作为最后的 URL，baseURL 将会被忽略。
- (NSString *)requestUrl;

///  Optional CDN URL for request.
///  供请求使用的可选的 CDN URL
- (NSString *)cdnUrl;

///  Requset timeout interval. Default is 60s.
///
///  @discussion When using `resumableDownloadPath`(NSURLSessionDownloadTask), the session seems to completely ignore
///              `timeoutInterval` property of `NSURLRequest`. One effective way to set timeout would be using
///              `timeoutIntervalForResource` of `NSURLSessionConfiguration`.
///  请求失效的时间间隔。默认为 60s。
///  讨论：当使用 resumableDownloadPath(NSURLSessionDownloadTask) 时，会话似乎完全忽略了 NSURLRequest 的 timeoutInterval 属性。一个有效的设置生效时间的方法是 NSURLSessionConfiguration 的 timeoutIntervalForResource
- (NSTimeInterval)requestTimeoutInterval;

///  Additional request argument.
///  其他请求参数
- (nullable id)requestArgument;

///  Override this method to filter requests with certain arguments when caching.
///  在缓存时重写此方法来过滤请求的某些参数
- (id)cacheFileNameFilterForRequestArgument:(id)argument;

///  HTTP request method.
///  HTTP 请求的方法
- (YTKRequestMethod)requestMethod;

///  Request serializer type.
///  请求序列化的类型
- (YTKRequestSerializerType)requestSerializerType;

///  Response serializer type. See also `responseObject`.
///  相应序列化的类型
- (YTKResponseSerializerType)responseSerializerType;

///  Username and password used for HTTP authorization. Should be formed as @[@"Username", @"Password"].
///  用户 HTTP 授权的用户名和密码。应该用 @[@"Username", @"Password"] 来构成
- (nullable NSArray<NSString *> *)requestAuthorizationHeaderFieldArray;

///  Additional HTTP request header field.
///  其他 HTTP 请求的头部信息
- (nullable NSDictionary<NSString *, NSString *> *)requestHeaderFieldValueDictionary;

///  Use this to build custom request. If this method return non-nil value, `requestUrl`, `requestTimeoutInterval`,
///  `requestArgument`, `allowsCellularAccess`, `requestMethod` and `requestSerializerType` will all be ignored.
/// 使用这个来构建自定义的请求。如果这个方法返回不为空的值，那么 requestUrl, requestTimeoutInterval,
/// requestArgument, allowscellularAccess, requestMethod 和 requestSerializerType 将会被忽略。
- (nullable NSURLRequest *)buildCustomUrlRequest;

///  Should use CDN when sending request.
///  在发送请求的时候是否应该使用 CDN
- (BOOL)useCDN;

///  Whether the request is allowed to use the cellular radio (if present). Default is YES.
/// 是否允许使用 蜂窝移动数据（如果有的话）。默认值为 YES
- (BOOL)allowsCellularAccess;

///  The validator will be used to test if `responseJSONObject` is correctly formed.
///  这个验证器将会被用作测试 responesJSONObject 被正确的构成
- (nullable id)jsonValidator;

///  This validator will be used to test if `responseStatusCode` is valid.
///  这个验证器将会被用于测试 responseStatusCode 是否是有效的
- (BOOL)statusCodeValidator;

@end

NS_ASSUME_NONNULL_END
