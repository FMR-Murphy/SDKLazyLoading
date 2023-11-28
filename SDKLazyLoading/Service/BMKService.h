//
//  BMKService.h
//  SDKLazyLoading
//
//  Created by Murphy on 2023/11/28.
//

#import <Foundation/Foundation.h>

@class FBLPromise<__covariant Value>;
@class BMKLocation;
NS_ASSUME_NONNULL_BEGIN

@interface BMKService : NSObject


+ (instancetype)service;

/// 初始化百度定位 SDK
- (FBLPromise *)setupBMK;

/// 获取定位信息 （单次定位）
- (FBLPromise<BMKLocation *> *)featchLocationData;

@end

NS_ASSUME_NONNULL_END
