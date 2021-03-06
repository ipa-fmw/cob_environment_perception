/*!
 *****************************************************************
 * \file
 *
 * \note
 *   Copyright (c) 2012 \n
 *   Fraunhofer Institute for Manufacturing Engineering
 *   and Automation (IPA) \n\n
 *
 *****************************************************************
 *
 * \note
 *  Project name: TODO FILL IN PROJECT NAME HERE
 * \note
 *  ROS stack name: TODO FILL IN STACK NAME HERE
 * \note
 *  ROS package name: TODO FILL IN PACKAGE NAME HERE
 *
 * \author
 *  Author: TODO FILL IN AUTHOR NAME HERE
 * \author
 *  Supervised by: TODO FILL IN CO-AUTHOR NAME(S) HERE
 *
 * \date Date of creation: TODO FILL IN DATE HERE
 *
 * \brief
 *
 *
 *****************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer. \n
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution. \n
 *     - Neither the name of the Fraunhofer Institute for Manufacturing
 *       Engineering and Automation (IPA) nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission. \n
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License LGPL as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License LGPL for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License LGPL along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************/
/*
 * registration_icp_narf.h
 *
 *  Created on: Nov 15, 2011
 *      Author: goa-jh
 */

#ifndef REGISTRATION_ICP_NARF_H_
#define REGISTRATION_ICP_NARF_H_

#include "registration_icp.h"
#include "features/narf.h"


namespace cob_3d_registration {

template <typename Point>
class Registration_ICP_NARF : public Registration_ICP_Features<Point>
{
  Feature_NARF<Point> *mom_;
  pcl::PointCloud<Point> tmp_input_;

public:
  Registration_ICP_NARF():
    mom_(new Feature_NARF<Point>)
  {
    this->setCorrDist(0.1);
    this->setFeatures(mom_);
  }

  virtual ~Registration_ICP_NARF() {
    delete mom_;
  }

  virtual boost::shared_ptr<const pcl::PointCloud<Point> > getMarkers() {return this->input_;}

protected:
  virtual bool compute_features() {
    //save input first
    tmp_input_ = *this->input_;

    //transform pc first
    if(!this->Registration_ICP_Features<Point>::compute_features())
      return false;

    mom_->setSearchRadius(this->icp_max_corr_dist_);
    if(this->register_.size()>0) {
      mom_->build(this->register_, *this->input_);
    }

    return true;
  }

  virtual bool compute_transformation() {
    ROS_INFO("compute_transformation");
    bool b;

    if(this->register_.size()>0) {
      pcl::PointCloud<Point> tmp = this->register_;

      //replace input cloud...
      this->register_= mom_->getFilteredInputCloud();
      this->input_  = mom_->getFilteredOutputCloud().makeShared();

      b=Registration_ICP_Features<Point>::compute_transformation();

      this->register_ = tmp;
      if(b) {
        pcl::transformPointCloud(tmp_input_, tmp_input_, this->transformation_);
        this->register_+= tmp_input_;
      }

    }
    else
      b=Registration_ICP_Features<Point>::compute_transformation();

    ROS_INFO("done");
    return b;
  }
};

}

#endif /* REGISTRATION_ICP_NARF_H_ */
