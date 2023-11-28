//
//  BMKService.m
//  SDKLazyLoading
//
//  Created by Murphy on 2023/11/28.
//

#import "BMKService.h"
#import <BMKLocationKit/BMKLocationComponent.h>
#if __has_include(<PromisesObjc/FBLPromises.h>)
#import <PromisesObjc/FBLPromises.h>
#else
#import <FBLPromise/FBLPromises.h>
#endif


static NSString * const BMKAppKey = @"MWG4SaVvFPrBtIP1iksSNGyW7XtLrXxq";

@interface BMKService () <BMKLocationAuthDelegate,BMKLocationManagerDelegate>

/// SDK 初始化
@property (nonatomic) FBLPromise * setupBMK;
/// 当前位置数据
@property (nonatomic) BMKLocation *locationData;
/// 定位管理
@property (nonatomic) BMKLocationManager *locationManager;
/// 定位权限管理
@property (nonatomic) CLLocationManager *manager;

@end

@implementation BMKService

+ (instancetype)service {
    static dispatch_once_t onceToken;
    static BMKService *service = nil;
    dispatch_once(&onceToken, ^{
        service = [[BMKService alloc] init];
    });
    return service;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        [self.manager requestWhenInUseAuthorization];
    }
    return self;
}

/// 百度定位 SDK 初始化
- (FBLPromise *)setupBMK {
    if (!_setupBMK) {
        _setupBMK = [FBLPromise async:^(FBLPromiseFulfillBlock  _Nonnull fulfill, FBLPromiseRejectBlock  _Nonnull reject) {
            NSLog(@"开始初始化百度定位 SDK");
            [BMKLocationAuth.sharedInstance setAgreePrivacy:YES];
            [BMKLocationAuth.sharedInstance checkPermisionWithKey:BMKAppKey authDelegate:self];
            fulfill(@YES);
        }];
    }
    return _setupBMK;
}

/// 获取定位信息 （单次定位）
- (FBLPromise<BMKLocation *> *)featchLocationData {
    __weak typeof(self) weakSelf = self;
    /// 调用 SDK 初始化
    return [[self setupBMK] then:^id _Nullable(id  _Nullable value) {
        return [FBLPromise onQueue:dispatch_get_main_queue() async:^(FBLPromiseFulfillBlock  _Nonnull fulfill, FBLPromiseRejectBlock  _Nonnull reject) {
            /// 开始定位
            __strong typeof(weakSelf) strongSelf = weakSelf;
            BOOL blockSucc = [strongSelf.locationManager requestLocationWithReGeocode:NO withNetworkState:NO completionBlock:^(BMKLocation * _Nullable location, BMKLocationNetworkState state, NSError * _Nullable error) {
                if (location) {
                    strongSelf.locationData = location;
                    fulfill(location);
                } else {
                    NSLog(@"%s -- %@", __func__, error);
                    reject(error);
                }
            }];
            /// 当上次定位未结束时，再次添加 block 会失败。
            NSLog(@"%@", blockSucc ? @"添加 block 成功": @"添加 block 失败");
        }];
    }];
}

#pragma mark - BMKLocationAuthDelegate
/**
 *@brief 返回授权验证错误
 *@param iError 错误号 : 为0时验证通过，具体参加BMKLocationAuthErrorCode
 */
- (void)onCheckPermissionState:(BMKLocationAuthErrorCode)iError {
    
}

#pragma mark - BMKLocationManagerDelegate
- (void)BMKLocationManager:(BMKLocationManager *)manager didUpdateLocation:(BMKLocation *)location orError:(NSError *)error {
    
}



#pragma mark - lazy
- (BMKLocationManager *)locationManager {
    if (!_locationManager) {
        _locationManager = [[BMKLocationManager alloc] init];
        _locationManager.delegate = self;
        _locationManager.coordinateType = BMKLocationCoordinateTypeBMK09LL;
        _locationManager.distanceFilter = kCLDistanceFilterNone;
        _locationManager.desiredAccuracy = kCLLocationAccuracyBest;
        _locationManager.activityType = CLActivityTypeOtherNavigation;
        _locationManager.pausesLocationUpdatesAutomatically = NO;
        _locationManager.allowsBackgroundLocationUpdates = NO;
        _locationManager.locationTimeout = 10;
        _locationManager.reGeocodeTimeout = 10;
    }
    return _locationManager;
}

@end
