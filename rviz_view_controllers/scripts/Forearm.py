#!/usr/bin/env python

import roslib
roslib.load_manifest("rviz_view_controllers")

import rospy
from math import *
from rviz_view_controllers.msg import CameraPlacement
from geometry_msgs.msg import Point, Vector3

rospy.init_node("camera_test", anonymous = True)

pub = rospy.Publisher("/rviz/camera_placement", CameraPlacement)

rate_float = 10
rate = rospy.Rate(rate_float)

rospy.Rate(2).sleep()

for i in range(3):
  
  t = rospy.get_time()
  cp = CameraPlacement()
 
  cp.attached_frame = "r_forearm_cam_frame"
 
  p = Point(0,0, 0)
  cp.eye.point = p
  cp.eye.header.frame_id = "r_forearm_cam_frame"
  
  f = Point(1, 0, 0)
  cp.focus.point = f
  cp.focus.header.frame_id = "r_forearm_cam_frame"


  up = Vector3(0,0,1)
  cp.up.vector = up
  cp.up.header.frame_id = "r_forearm_cam_frame"

  cp.time_from_start = rospy.Duration(1)  
  print "Publishing a message!"
  pub.publish(cp)
  #print "Sleeping..."
  rate.sleep()
  #print "End of loop!"

