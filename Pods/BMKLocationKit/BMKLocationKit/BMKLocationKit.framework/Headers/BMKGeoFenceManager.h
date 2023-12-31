//
//  BMKGeoFenceManager.h
//  BMKLocationKit
//
//  Created by baidu on 2017/3/2.
//  Copyright © 2017年 baidu. All rights reserved.
//

#import "BMKGeoFenceRegion.h"
#import "BMKLocation.h"

@protocol BMKGeoFenceManagerDelegate;

///地理围栏监听状态类型
typedef NS_OPTIONS(NSUInteger, BMKGeoFenceActiveAction)
{
    BMKGeoFenceActiveActionNone     = 0,       ///< 不进行监听
    BMKGeoFenceActiveActionInside   = 1 << 0,  ///< 在范围内
    BMKGeoFenceActiveActionOutside  = 1 << 1,  ///< 在范围外
    BMKGeoFenceActiveActionStayed   = 1 << 2,  ///< 停留(在范围内超过10分钟)
};

///BMKGeoFence errorDomain
FOUNDATION_EXPORT NSErrorDomain const _Nonnull BMKGeoFenceErrorDomain;

///地理围栏错误码
typedef NS_ENUM(NSInteger, BMKGeoFenceErrorCode) {
    BMKGeoFenceErrorUnknown = 1,                    ///< 未知错误
    BMKGeoFenceErrorInvalidParameter = 2,           ///< 参数错误
    BMKGeoFenceErrorFailureConnection = 3,          ///< 网络连接异常
    BMKGeoFenceErrorFailureAuth  = 4,               ///< 鉴权失败
    BMKGeoFenceErrorNoValidFence = 5,               ///< 无可用围栏
    BMKGeoFenceErroFailureLocating = 6,             ///< 定位错误
};


///地理围栏管理类
@interface BMKGeoFenceManager : NSObject


///实现了 BMKGeoFenceManagerDelegate 协议的类指针。
@property (nonatomic, weak, nullable) id<BMKGeoFenceManagerDelegate> delegate;


///需要进行通知的行为，默认为BMKGeoFenceActiveActionInside。
@property (nonatomic, assign) BMKGeoFenceActiveAction activeAction;


///设定定位的最小更新距离。默认为 kCLDistanceFilterNone。
@property(nonatomic, assign) CLLocationDistance distanceFilter;


///设定定位精度。默认为 kCLLocationAccuracyBest。
@property(nonatomic, assign) CLLocationAccuracy desiredAccuracy;


///设定定位类型。默认为 CLActivityTypeAutomotiveNavigation。
@property(nonatomic, assign) CLActivityType activityType;


///指定定位是否会被系统自动暂停。默认为NO。
@property (nonatomic, assign) BOOL pausesLocationUpdatesAutomatically;


///是否允许后台定位。默认为NO。只在iOS 9.0及之后起作用。设置为YES的时候必须保证 Background Modes 中的 Location updates 处于选中状态，否则会抛出异常。
@property (nonatomic, assign) BOOL allowsBackgroundLocationUpdates;


/**
 * @brief 添加一个圆形围栏
 * @param center 围栏的中心点经纬度坐标
 * @param radius 围栏的半径，单位：米，要求大于0
 * @param type 围栏的坐标系类型
 * @param customID 用户自定义ID，可选，SDK原值返回
 */
- (void)addCircleRegionForMonitoringWithCenter:(CLLocationCoordinate2D)center radius:(CLLocationDistance)radius coorType:(BMKLocationCoordinateType)type customID:(NSString * _Nullable)customID;


/**
 * @brief 根据经纬度坐标数据添加一个闭合的多边形围栏，点与点之间按顺序尾部相连, 第一个点与最后一个点相连
 * @param coordinates 经纬度坐标点数据,coordinates对应的内存会拷贝,调用者负责该内存的释放
 * @param count 经纬度坐标点的个数，不可小于3个
 * @param type 围栏的坐标系类型
 * @param customID 用户自定义ID，可选，SDK原值返回
 */
- (void)addPolygonRegionForMonitoringWithCoordinates:(CLLocationCoordinate2D * _Nonnull)coordinates count:(NSInteger)count coorType:(BMKLocationCoordinateType)type customID:(NSString * _Nullable)customID;


/**
 * @brief 根据customID获得指定的围栏，如果customID传nil，则返回全部围栏
 * @param customID 用户执行添加围栏函数时传入的customID
 * @return 获得的围栏构成的数组，如果没有结果，返回nil
 */
- (NSArray * _Nullable)geoFenceRegionsWithCustomID:(NSString * _Nullable)customID;


/**
 * @brief 移除指定围栏
 * @param region 要停止监控的围栏
 */
- (void)removeTheGeoFenceRegion:(BMKGeoFenceRegion * _Nonnull)region;


/**
 * @brief 移除指定customID的围栏
 * @param customID 用户执行添加围栏函数时传入的customID
 */
- (void)removeGeoFenceRegionsWithCustomID:(NSString * _Nullable)customID;


/**
 * @brief 移除所有围栏
 */
- (void)removeAllGeoFenceRegions;


@end



///地理围栏代理协议,该协议定义了获取地理围栏相关回调方法，包括添加、状态改变等。
@protocol BMKGeoFenceManagerDelegate <NSObject>

@optional

/**
 *  @brief 为了适配app store关于新的后台定位的审核机制（app store要求如果开发者只配置了使用期间定位，则代码中不能出现申请后台定位的逻辑），当开发者在plist配置NSLocationAlwaysUsageDescription或者NSLocationAlwaysAndWhenInUseUsageDescription时，需要在该delegate中调用后台定位api：[locationManager requestAlwaysAuthorization]。开发者如果只配置了NSLocationWhenInUseUsageDescription，且只有使用期间的定位需求，则无需在delegate中实现逻辑。
 *  @param manager 定位 BMKGeoFenceManager 类。
 *  @param locationManager 系统 CLLocationManager 类 。
 *  @since 1.7.0
 */
- (void)BMKGeoFenceManager:(BMKGeoFenceManager * _Nonnull)manager doRequestAlwaysAuthorization:(CLLocationManager * _Nonnull)locationManager;

/**
 * @brief 添加地理围栏完成后的回调，成功与失败都会调用
 * @param manager 地理围栏管理类
 * @param regions 成功添加的一个或多个地理围栏构成的数组
 * @param customID 用户执行添加围栏函数时传入的customID
 * @param error 添加失败的错误信息
 */
- (void)BMKGeoFenceManager:(BMKGeoFenceManager * _Nonnull)manager didAddRegionForMonitoringFinished:(NSArray <BMKGeoFenceRegion *> * _Nullable)regions customID:(NSString * _Nullable)customID error:(NSError * _Nullable)error;


/**
 * @brief 地理围栏状态改变时回调，当围栏状态的值发生改变，定位失败都会调用
 * @param manager 地理围栏管理类
 * @param region 状态改变的地理围栏
 * @param customID 用户执行添加围栏函数时传入的customID
 * @param error 错误信息，如定位相关的错误
 */
- (void)BMKGeoFenceManager:(BMKGeoFenceManager * _Nonnull)manager didGeoFencesStatusChangedForRegion:(BMKGeoFenceRegion * _Nullable)region customID:(NSString * _Nullable)customID error:(NSError * _Nullable)error;

/**
 * @brief 地理围栏内部的连续定位回调，如需停止定位请移除所有地理围栏
 * @param manager 地理围栏管理类
 * @param location 定位结果，参考BMKLocation。
 * @param error 错误信息
 */
- (void)BMKGeoFenceManager:(BMKGeoFenceManager * _Nonnull)manager didUpdateLocation:(BMKLocation * _Nullable)location orError:(NSError * _Nullable)error;

/**
 *  @brief 当定位发生错误时，会调用代理的此方法。
 *  @param manager BMKGeoFenceManager 类。
 *  @param error 返回的错误，参考 CLError 。
 */
- (void)BMKGeoFenceManager:(BMKGeoFenceManager * _Nonnull)manager didFailWithError:(NSError * _Nullable)error;
@end

