//
//  BMKLocationAuth.h
//  LocationComponent
//
//  Created by baidu on 2017/4/10.
//  Copyright © 2017年 baidu. All rights reserved.
//

#ifndef BMKLocationAuth_h
#define BMKLocationAuth_h

///定位鉴权错误码
typedef NS_ENUM(NSInteger, BMKLocationAuthErrorCode) {
    BMKLocationAuthErrorUnknown = -1,                    ///< 未知错误
    BMKLocationAuthErrorSuccess = 0,           ///< 鉴权成功
    BMKLocationAuthErrorNetworkFailed = 1,          ///< 因网络鉴权失败
    BMKLocationAuthErrorFailed  = 2,               ///< KEY非法鉴权失败

};
///通知Delegate
@protocol BMKLocationAuthDelegate <NSObject>
@optional

/**
 *@brief 返回授权验证错误
 *@param iError 错误号 : 为0时验证通过，具体参加BMKLocationAuthErrorCode
 */
- (void)onCheckPermissionState:(BMKLocationAuthErrorCode)iError;
@end


///BMKLocationAuth类。用于鉴权
@interface BMKLocationAuth : NSObject

///鉴权状态0：成功； 1：网络错误； 2：授权失败
@property(nonatomic, readonly, assign) BMKLocationAuthErrorCode permisionState __deprecated_msg("已废弃since 2.0.5");

///是否同意隐私合规政策
@property(nonatomic, readonly, assign) BOOL isAgreePrivacy;

/**
 * @brief 得到BMKLocationAuth的单例
 */
+ (BMKLocationAuth*)sharedInstance;


/**
 *@brief 启动引擎
 *@param key 申请的有效key
 *@param delegate 回调是否鉴权成功
 */
-(void)checkPermisionWithKey:(NSString*)key authDelegate:(id<BMKLocationAuthDelegate>)delegate;


/**
 *@brief 更新是否同意隐私合格政策需要在BMKLocationManager和BMKGeoFenceManager实例化之前调用，否则实例化失败，定位功能不可用。
 *       在使用BMKLocationManager和BMKGeoFenceManager时，请注意需要加判空处理。
 *       隐私政策官网链接：https://lbsyun.baidu.com/index.php?title=openprivacy
 *
 *       note:隐私政策变更后需要重新初始化BMKLocationManager或BMKGeoFenceManager
 *@param agreePrivacy 是否同意
 */
- (void)setAgreePrivacy:(BOOL)agreePrivacy;

@end



#endif /* BMKLocationAuth_h */
