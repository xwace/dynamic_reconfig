#include <ros/ros.h>
#include <dynamic_reconfigure/server.h>
//#include <dynamic_reconfigure/BoolParameter.h>
//#include <dynamic_reconfigure/IntParameter.h>
//#include <dynamic_reconfigure/Reconfigure.h>
//#include <dynamic_reconfigure/Config.h>
#include <dynReconfig/dynamicparamConfig.h>
#include <iostream>
#include <mutex>

std::mutex mtx;
//Server服务端的方式回调获取GUI界面的动态参数
void callback(dynamic_tutorial::dynamicparamConfig &config, uint32_t level){
	    mtx.lock();
            ROS_INFO("Reconfigure Request: %d %f %s %s %d", 
                    config.int_param, config.double_param, 
                    config.str_param.c_str(), 
                    config.bool_param?"True":"False", 
                    config.size);
            mtx.unlock();
}

//subscribe多线程的方式读取GUI界面发布的话题"parameter_updates"
void dynamic_reconfigure_current_config_callback(const dynamic_reconfigure::ConfigConstPtr& current_config)
{
	
    
    dynamic_reconfigure::ReconfigureRequest req;
    dynamic_reconfigure::ReconfigureResponse res;

    req.config = *current_config;

    std::cout<<"into the cb"<<std::endl;

    for(auto cfg:req.config.ints) std::cout<<"cfg: "<<cfg<<std::endl; 
    for(auto cfg:req.config.strs) std::cout<<"cfg: "<<cfg<<std::endl;
    for(auto cfg:req.config.bools) std::cout<<"cfg: "<<cfg<<std::endl;
    for(auto cfg:req.config.doubles) std::cout<<"cfg: "<<cfg<<std::endl;

    //if(ros::service::waitForService("dynamic_tutorials/set_parameters",3000)){
    if(ros::service::exists("dynamic_tutorials/set_parameters",true)){
        std::cout<<"Find the service"<<std::endl;
        //下面这行代码会导致程序卡死,原因可能是request输入的格式有误,导致无法获取到服务端的响应
        //const bool success = ros::service::call("dynamic_tutorials/set_parameters", req, res);
        //std::cout<<"Done the cb: "<<success<<std::endl;
    }else std::cout<<"Failed to call the service!!!"<<std::endl;
    ros::spin();
    
}

int main(int argc, char** argv)
{
    
    ros::init(argc,argv,"dynamic_tutorials");
    ros::NodeHandle node_handle;
    ros::NodeHandle nh("~");

    dynamic_reconfigure::Server<dynamic_tutorial::dynamicparamConfig> server;
    dynamic_reconfigure::Server<dynamic_tutorial::dynamicparamConfig>::CallbackType f;
    f = boost::bind(&callback,_1,_2);
    server.setCallback(f);

    ROS_INFO("spinning node");
    ros::Subscriber dynamic_reconfigure_sub_ = node_handle.subscribe("dynamic_tutorials/parameter_updates", 1, &dynamic_reconfigure_current_config_callback);
    ROS_INFO("subscribe node");

    ros::spin();
    return 0;
}
