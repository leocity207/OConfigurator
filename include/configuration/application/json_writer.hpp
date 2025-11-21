#ifndef CONFIGURATION_APPLICATION_JSON_WRITER_HPP
#define CONFIGURATION_APPLICATION_JSON_WRITER_HPP

#include <optional>
#include <fstream>
#include <string>
#include <system_error>
#include <type_traits>

#include "container.h"
#include "json_writer.h"

#include "include/tuple_helper.h"

#include <rapidjson/document.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>



// We'll write to "configuration.json" in current working directory.
// Replace the literal path below or change the function signature if you need a custom path.
template<class... Data_Modules>
std::optional<O::Configuration::Application::Write_Error>
O::Configuration::Application::Write_As_JSON_File(
    const O::Configuration::Application::Container<Data_Modules...>& datas,
    const std::filesystem::path& filepath)
{
    FILE* fp = std::fopen(filepath.string().c_str(), "wb");
    if (!fp)
        return Write_Error::FILE_OPEN_FAILED;

    char buffer[65536];
    rapidjson::FileWriteStream os(fp, buffer, sizeof(buffer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);

    writer.StartObject();

    O::For_Each_In_Tuple(datas.modules, [&](auto const& module_part) {
        using Module_T = std::decay_t<decltype(module_part)>;
        using WriterT  = typename O::Configuration::Module::Traits<Module_T>::Writer;

        WriterT module_writer;

        writer.Key(WriterT::Key());
        module_writer.To_JSON(writer, module_part);
    });

    writer.EndObject();

    std::fclose(fp);
    return std::nullopt; // success
}

template<class... Data_Modules>
std::string O::Configuration::Application::Write_As_JSON_String(const O::Configuration::Application::Container<Data_Modules...>& datas)
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

    writer.StartObject();

    O::For_Each_In_Tuple(datas.modules, [&](auto const& module_part) {
        using Module_T = std::decay_t<decltype(module_part)>;
        using WriterT  = typename O::Configuration::Module::Traits<Module_T>::Writer;

        WriterT module_writer;

        writer.Key(WriterT::Key());
        module_writer.To_JSON(writer, module_part);
    });

    writer.EndObject();
    return sb.GetString();
}

#endif