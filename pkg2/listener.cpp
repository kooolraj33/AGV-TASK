#include "ros/ros.h"
#include "geometry_msgs/Point.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

struct makepath{
		int x;
		int y;
}mkpath[200];

Mat img1 = imread("ps1.jpg",CV_LOAD_IMAGE_COLOR);
int sum1_x=0,sum1_y=0,count1=0,at_x,at_y; 

void makeblue(float x1, float y1){
  img1.at<Vec3b>(x1,y1).val[0]=255;
  img1.at<Vec3b>(x1,y1).val[1]=0;
  img1.at<Vec3b>(x1,y1).val[2]=0;
  imshow("path",img1);
}

void chatterCallback(const geometry_msgs::Point::ConstPtr& msg)
{
  int i=0;
  int x=0,y=0;
  geometry_msgs:: Point r;

  r.x = msg->x;
  r.y = msg->y;
  // ROS_INFO("%f %f", r.x, r.y);
  makeblue(r.x,r.y);
  imshow("path",img1);
  if(r.x==(at_x+1) && r.y==(at_y+1)){
    cout<<"Path has been created!";
    waitKey(0);
  }
}

int main(int argc, char **argv)
{   
  cout<<"Retrieving Data...Please Wait\n ";
  namedWindow("path", WINDOW_NORMAL);
  int i=0,j=0; 
    for(i=0;i<img1.rows;i++){
      for(j=0;j<img1.cols;j++){
        if(img1.at<Vec3b>(i,j)[2] > 200  && img1.at<Vec3b>(i,j)[0] < 100){
	  sum1_x+=i;
	  sum1_y+=j;
          count1++;
        }
      }
    }
     at_x = sum1_x/count1;
     at_y = sum1_y/count1;
    ros::init(argc, argv, "subscriber");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("speaker", 1000, chatterCallback);
    ros::spin();
    return 0;
}
