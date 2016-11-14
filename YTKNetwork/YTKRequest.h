//
//  YTKRequest.h
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

#import "YTKBaseRequest.h"

NS_ASSUME_NONNULL_BEGIN

/*
如果项目中涉及到使用C++语言的编程的话则最好使用
FOUNDATION_EXPORT
FOUNDATION_IMPORT
否则 使用
FOUNDATION_EXTERN 这个就可以了
当然使用extern 也是没有问题的，就看大家的需求了
*/
FOUNDATION_EXPORT NSString *const YTKRequestCacheErrorDomain;


NS_ENUM(NSInteger) {
    YTKRequestCacheErrorExpired = -1,
    YTKRequestCacheErrorVersionMismatch = -2,
    YTKRequestCacheErrorSensitiveDataMismatch = -3,
    YTKRequestCacheErrorAppVersionMismatch = -4,
    YTKRequestCacheErrorInvalidCacheTime = -5, // 无效的缓存时间
    YTKRequestCacheErrorInvalidMetadata = -6,   // metadata 缓存生效
    YTKRequestCacheErrorInvalidCacheData = -7,
};

///  YTKRequest is the base class you should inherit to create your own request class.
///  Based on YTKBaseRequest, YTKRequest adds local caching feature. Note download
///  request will not be cached whatsoever, because download request may involve complicated
///  cache control policy controlled by `Cache-Control`, `If-Modified-Since`, etc.
/**
 YTKRequest
 */
@interface YTKRequest : YTKBaseRequest

///  Whether to use cache as response or not.
///  Default is NO, which means caching will take effect with specific arguments.
///  Note that `cacheTimeInSeconds` default is -1. As a result cache data is not actually
///  used as response unless you return a positive value in `cacheTimeInSeconds`.
///
///  Also note that this option does not affect storing the response, which means response will always be saved
///  even `ignoreCache` is YES.
///  是否使用缓存作为结果
///  默认为 NO，也就是在具体的参数配置下，缓存将会对结果产生影响
///  注意：'cacheTimeInSeconds' 默认为 -1. 因此缓存数据并不会真正的作为返回结果，除非给 'cacheTimeInSeconds' 设置一个正数
///  注意：这个设置并不会影响存储返回结果。即使 'ignoreCache' 是 YES， 返回结果依然会被保存。
@property (nonatomic) BOOL ignoreCache;

///  Whether data is from local cache.
///  是否从本地缓存中获取的数据
- (BOOL)isDataFromCache;

///  Manually load cache from storage.
///
///  @param error If an error occurred causing cache loading failed, an error object will be passed, otherwise NULL.
///
///  @return Whether cache is successfully loaded.
///  手动从存储当中缓存
///
///  error 如果出现一个引起加载缓存失败的错误，这个错误将会被传递，否则为 NULL
///
///  缓存是否被成功加载
- (BOOL)loadCacheWithError:(NSError * __autoreleasing *)error;

///  Start request without reading local cache even if it exists. Use this to update local cache.
///  开始请求，并且即使本地有缓存也不会去读取。用这个去更新本地的缓存。
- (void)startWithoutCache;

///  Save response data (probably from another request) to this request's cache location
///  保存结果数据（可能是其他请求的）到这个请求的缓存处
- (void)saveResponseDataToCacheFile:(NSData *)data;

#pragma mark - Subclass Override 子类重写

///  The max time duration that cache can stay in disk until it's considered expired.
///  Default is -1, which means response is not actually saved as cache.
///  缓存可以保存在硬盘内的最大有效时间
///  默认值是 -1，这意味着返回结果并不会保存作为缓存
- (NSInteger)cacheTimeInSeconds;

///  Version can be used to identify and invalidate local cache. Default is 0.
///  版本可以用来标示，或始本地缓存作废。默认值为 0
- (long long)cacheVersion;

///  This can be used as additional identifier that tells the cache needs updating.
///
///  @discussion The `description` string of this object will be used as an identifier to verify whether cache
///              is invalid. Using `NSArray` or `NSDictionary` as return value type is recommended. However,
///              If you intend to use your custom class type, make sure that `description` is correctly implemented.
///  这个可以被用作额外的标示，来指示缓存需要更新
///
///  这个对象的 'description' 字符串将会被用作检测缓存数据是否有效的标记。建议使用 'NSArray' 或者 'NSDictionary' 作为返回值。
///  但是，如果尝试用自定义的类型，需要确保 'description' 可以正确的执行。
- (nullable id)cacheSensitiveData;

///  Whether cache is asynchronously written to storage. Default is YES.
///  缓存是否异步的写到存储。默认为 YES
- (BOOL)writeCacheAsynchronously;

@end

NS_ASSUME_NONNULL_END
