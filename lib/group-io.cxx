#include "../include/casmutils/sym/group-io.hpp"
#include <iostream>
#include <fstream>
#include <casmutils/xtal/structure.hpp>
#include <casmutils/xtal/structure_tools.hpp>
#include <casmutils/sym/symgroup.hpp>

namespace cu=casmutils;

nlohmann::json convert_group_to_json(const std::vector<cu::sym::CartOp>& group){

    nlohmann::json group_json, symop;
    
    //loop over every CartOp in group, 
    //put elements into symop json (temp),
    //pushback on group json
    int ct=0;
    for(const cu::sym::CartOp& next_op : group){
        // make labels lowercase_with_underscore?? Whats the the CASM standard
        symop["symop_number"] = ct;
        ct++;
        Eigen::Matrix3d matrix=next_op.matrix;
        Eigen::Vector3d translation = next_op.translation;
        symop["row_matrix"]={matrix(0,0), matrix(0,1), matrix(0,2),
                        matrix(1,0), matrix(1,1), matrix(1,2), 
                        matrix(2,0), matrix(2,1), matrix(2,2)};
        symop["translation"] = {translation(0), translation(1),translation(2)};
        symop["time_reversal"] = next_op.is_time_reversal_active;
        group_json.push_back(symop);
    }
    return group_json;
}

std::vector<cu::sym::CartOp> convert_json_to_group(const nlohmann::json& group_json){
    std::vector<cu::sym::CartOp> group_vector;
    for(const auto& temp : group_json){
        auto mat_vector=temp["row_matrix"].get<std::vector<float>>();
        Eigen::Matrix3d matrix;
        matrix<< mat_vector[0], mat_vector[1],mat_vector[2], mat_vector[3],mat_vector[4], mat_vector[5],mat_vector[6], mat_vector[7],mat_vector[8]; 
        Eigen::Vector3d translation;
        auto trans_vector=temp["translation"].get<std::vector<float>>();
        translation<<trans_vector[0], trans_vector[1], trans_vector[2];
        cu::sym::CartOp temp_op(matrix, translation, temp["time_reversal"]);
        group_vector.push_back(temp_op);
    }
    return group_vector;
}

std::vector<cu::sym::CartOp> read_group_from_file(const cu::fs::path& group_json_path)
{
    std::ifstream stream(group_json_path);
    nlohmann::json group_json;
    stream>>group_json;
    return convert_json_to_group(group_json);
}

void print_group_to_file(const std::vector<cu::sym::CartOp>& group, const cu::fs::path& output_path)
{   
    nlohmann::json group_json = convert_group_to_json(group);
    std::ofstream ofs(output_path);
    ofs<<group_json.dump(group_json.size());
    ofs.close();
    return;
}

