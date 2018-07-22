//
// Created by pairs on 7/11/18.
//

#pragma once

namespace zros {

    template <typename T>
    std::string serializeToString(const T & object) {
        static_assert(std::is_base_of<::google::protobuf::Message, T>::value
                      || std::is_base_of<std::string, T>::value, "Unsupported type");

        if (typeid(T) == typeid(std::string)) {
            return ((const std::string &)object);
        } else if (std::is_base_of<::google::protobuf::Message, T>::value) {
            return ((const ::google::protobuf::Message &) object).SerializeAsString();
        }
    }

    template <typename T>
    void parseFromString(const std::string & data, T *object) {
        static_assert(std::is_base_of<::google::protobuf::Message, T>::value
                      || std::is_base_of<std::string, T>::value, "Unsupported type");

        if (typeid(T) == typeid(std::string)) {
            *((std::string *)object) = data;
        } else if (std::is_base_of<::google::protobuf::Message, T>::value) {
            ((::google::protobuf::Message *)object)->ParseFromString(data);
        }
    }
}
