#pragma once

#include <zen/log.hpp>

#include <cstdint>
#include <format>
#include <string>

namespace xgl::config {

    struct Version final {
    public:
        uint8_t major; uint8_t minor;

        bool operator==(Version other) const noexcept
        {
            return this->major == other.major && this->minor == other.minor;
        }

        friend constexpr std::strong_ordering operator<=>(const Version& self, const Version& other) noexcept
        {
            const std::strong_ordering CMP = self.major <=> other.major;
            return (CMP != 0) ? CMP : (self.minor <=> other.minor);
        }
    };

    inline constexpr Version  VERSION  { 0, 1 };
    inline const zen::log_tag XGL_TAG { "XGL", zen::ansi_color::MAGENTA };

} // namespace xgl::config

template <> struct std::formatter<xgl::config::Version> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    auto format(const xgl::config::Version& type, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "{}.{}", type.major, type.minor);
    }
};
