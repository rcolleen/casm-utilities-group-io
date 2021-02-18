#ifndef GROUP_IO_H
#define GROUP_IO_H

#include <nlohmann/json.hpp>
#include <vector>
#include <casmutils/definitions.hpp>
#include <casmutils/sym/cartesian.hpp>

namespace cu = casmutils;

nlohmann::json convert_group_to_json(const std::vector<cu::sym::CartOp>& group);

std::vector<cu::sym::CartOp> convert_json_to_group( const nlohmann::json& group_json);

std::vector<cu::sym::CartOp> read_group_from_file( const cu::fs::path& group_json_path);

void print_group_to_file(const std::vector<cu::sym::CartOp>& group, const cu::fs::path& output_file_path);

#endif
