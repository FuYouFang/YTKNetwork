//
//  te.m
//  YTKNetworkDemo
//
//  Created by fuyoufang on 2016/11/10.
//  Copyright © 2016年 yuantiku.com. All rights reserved.
//

#import "te.h"

//@implementation te
//
//@end
///*
// #import "PanView.h"
// 
// #pragma mark - Helper Functions
// 
// /*获得透明偏移*/
//static NSUInteger alphaOffset(NSUInteger pointX, NSUInteger pointY,  NSUInteger width) {
//    return pointY * width * 4 + pointX * 4 + 0;
//}
//
//NSData *getBitmapDataFromImage(UIImage *sourceImage){
//    //No.1
//    //开始写代码,获取图片的位图数据。
//    
//    

//    //end_code
//}
//
//
//@implementation PanView{
//    
//    CGPoint previousLocation;
//    NSData * data;
//}
//
//- (instancetype)initWithImage:(UIImage *)image{
//    
//    if (self = [super initWithImage:image]){
//        self.userInteractionEnabled = YES;
//        UIPanGestureRecognizer *panRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
//        [self addGestureRecognizer:panRecognizer];
//        
//        data = getBitmapDataFromImage(image);
//    }
//    return self;
//}
//
//#pragma mark - Touches & Gesture
//
////No.2
////开始写代码,判断触摸的点是否在当前的view中（默认像素的透明度在30%以上才认定视图被点击）。
//- (BOOL)
//
//
//
//
//
//
//
//
////end_code
//
//- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
//    [self.superview bringSubviewToFront:self];
//    previousLocation = self.center;
//}
//
//- (void)handlePan:(UIPanGestureRecognizer *)uigr{
//    CGPoint translation = [uigr translationInView:self.superview];
//    CGPoint newcenter = CGPointMake(previousLocation.x + translation.x, previousLocation.y + translation.y);
//    
//    float halfx = CGRectGetMidX(self.bounds);
//    newcenter.x = MAX(halfx, newcenter.x);
//    newcenter.x = MIN(self.superview.bounds.size.width - halfx, newcenter.x);
//    
//    float halfy = CGRectGetMidY(self.bounds);
//    newcenter.y = MAX(halfy, newcenter.y);
//    newcenter.y = MIN(self.superview.bounds.size.height - halfy, newcenter.y);
//    
//    self.center = newcenter;
//}
//
//@end
//*/
