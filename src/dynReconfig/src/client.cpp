#include <ros/ros.h>
#include <dynamic_reconfigure/server.h>
#include <dynReconfig/dynamicparamConfig.h>
#include <iostream>
#include <mutex>

void dynamic_reconfigure_current_config_callback(const dynamic_reconfigure::ConfigConstPtr& current_config)
{   
    dynamic_reconfigure::ReconfigureRequest req;
    dynamic_reconfigure::ReconfigureResponse res;
    dynamic_reconfigure::Config conf;
    dynamic_reconfigure::IntParameter ints;

    //手动修改reconfigure GUI界面的参数,比如设置int_param为75(0-100),当运行该节点时,GUI显示为75
    conf = *current_config;
    ints.name = "int_param";
    ints.value = 75;
    conf.ints.clear();//记得清空;否则无法更改写入75
    conf.ints.push_back(ints);
    req.config = conf;

    //查看GUI的所有参数
    //for(auto cfg:req.config.ints) std::cout<<"cfg: "<<cfg<<std::endl; 
    //for(auto cfg:req.config.strs) std::cout<<"cfg: "<<cfg<<std::endl;
    //for(auto cfg:req.config.bools) std::cout<<"cfg: "<<cfg<<std::endl;
    //for(auto cfg:req.config.doubles) std::cout<<"cfg: "<<cfg<<std::endl;

    ros::service::waitForService("dynamic_tutorials/set_parameters");
    std::cout<<"wait for service successfully!!"<<std::endl;
    const bool success = ros::service::call("dynamic_tutorials/set_parameters", req, res);//if(ros::service::exists("dynamic_tutorials/set_parameters",true))
    std::cout<<"finish the call back"<<std::endl;
    
}


int main(int argc, char** argv)
{
    
    ros::init(argc,argv,"ws");
    ros::NodeHandle node_handle;
    ros::NodeHandle nh("~");

    //客户端,请求别人做某事
    ROS_INFO("spinning node");
    ros::Subscriber dynamic_reconfigure_sub_ = node_handle.subscribe("/dynamic_tutorials/parameter_updates", 1, dynamic_reconfigure_current_config_callback);
    ros::Rate loop_rate(1);
    while(ros::ok()){
	ros::spinOnce();
	loop_rate.sleep(); 
    }

    ROS_INFO("subscribe node");

    //ros::spin();//无法控制频率
    return 0;
}
