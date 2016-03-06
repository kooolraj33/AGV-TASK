#include "ros/ros.h"
#include "geometry_msgs/Point.h"
#include <cv_bridge/cv_bridge.h>
#include <iostream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

struct graphs{
		int x;
		int y;
	       }graph[200];

Mat img1 = imread("ps1.jpg",CV_LOAD_IMAGE_COLOR);
int r,c;
  int a[200][200];

 int visited[200][200];
int flag=0,m=0; 
  int end_x,end_y;
 
void dfs(int at_x,int at_y)
{
  visited[at_x][at_y]=1;
  int i=0,j=0;
  for(i=1;i>-2;i--){	
    for(j=1;j>-2;j--){
      if(at_x+i>=0 && at_y+j>=0 && visited[at_x+i][at_y+j]==0 && a[at_x+i][at_y+j]!=0 && flag==0 && at_x+i<=end_x && at_y+j<=end_y){
	dfs(at_x+i,at_y+j);
	if(flag==1)
	  break;
      }
    }
    if(flag==1)
      break;
  }
  if(at_x==end_x && at_y==end_y){
    flag=1;
    return;
  }
  if(flag==1)
  {
    graph[m].x=at_x + i;
    graph[m].y=at_y + j;
    m++;                 
  }                        
}

int main(int argc, char **argv)
{
  int sum1_x=0,sum1_y=0,sum2_x=0,sum2_y=0;
  long  int st_x,st_y; 
  int count1=0,count2=0;
  int i=0,j=0,temp=0;
  Mat img = imread("ps1.jpg",CV_LOAD_IMAGE_COLOR);
  r = img.rows;
  c = img.cols;  
  for(i=0;i<200;i++)
    for(j=0;j<200;j++)
      visited[i][j]=1;

  for(i=0;i<r;i++){
    for(j=0;j<c;j++){
      if(img.at<Vec3b>(i,j)[2] > 200  && img.at<Vec3b>(i,j)[0] < 100){
	sum1_x+=i;
	sum1_y+=j;
        count1++;
      }
      if(img.at<Vec3b>(i,j)[1] > 200 && img.at<Vec3b>(i,j)[2] < 10){
        sum2_x+=i;
	sum2_y+=j;
        count2++;
      }
      if(img.at<Vec3b>(i,j)[0]>200 && img.at<Vec3b>(i,j)[1]>200 
	 && img.at<Vec3b>(i,j)[2]>200){
         a[i][j]=0;
      }
      else{
	a[i][j]=1;
      }
    }
  }

  st_x = sum1_x/count1;
  st_y = sum1_y/count1;
  end_x = sum2_x/count2;
  end_y = sum2_y/count2;
  
  a[st_x][st_y]=1;
  a[end_x][end_y]=1;
  dfs(st_x,st_y);
 
  ros::init(argc, argv, "publisher"); 
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<geometry_msgs::Point>("speaker", 1000);
  ros::Rate loop_rate(10);

  while (temp<m)
  {
    geometry_msgs:: Point msg;
    msg.x = graph[temp].x;
    msg.y = graph[temp].y ;
    cout<<"..."<<temp<<endl;
    chatter_pub.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();
    temp++;
  }
  return 0;
}
