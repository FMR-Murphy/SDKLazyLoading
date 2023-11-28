//
//  ViewController.m
//  SDKLazyLoading
//
//  Created by Murphy on 2023/11/26.
//

#import "ViewController.h"

#import "BMKService.h"
#import <BMKLocationKit/BMKLocationComponent.h>

#if __has_include(<PromisesObjc/FBLPromises.h>)
#import <PromisesObjc/FBLPromises.h>
#else
#import <FBLPromise/FBLPromises.h>
#endif

@interface ViewController () <CLLocationManagerDelegate>


@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor grayColor];
    
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    NSLog(@"开始请求定位");
    [[BMKService.service featchLocationData]then:^id _Nullable(BMKLocation * _Nullable value) {
        NSLog(@"定位成功 - latitude:%f - longitude:%f", value.location.coordinate.latitude, value.location.coordinate.longitude);
        return nil;
    }];
}

@end
