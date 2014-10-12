/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2013 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/
#ifndef MAPNIK_TEXT_PROPERTIES_HPP
#define MAPNIK_TEXT_PROPERTIES_HPP

// mapnik
#include <mapnik/color.hpp>
#include <mapnik/attribute.hpp>
#include <mapnik/font_set.hpp>
#include <mapnik/expression.hpp>
#include <mapnik/text/formatting/base.hpp>
#include <mapnik/symbolizer_base.hpp>
#include <mapnik/symbolizer_enumerations.hpp>
#include <mapnik/noncopyable.hpp>
#include <mapnik/text/font_feature_settings.hpp>

// stl
#include <map>

// boost
#include <boost/optional.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

namespace mapnik {

class feature_impl;
class text_layout;

namespace detail {

struct evaluated_format_properties
{
    evaluated_format_properties() :
      face_name(),
      text_size(0.0),
      character_spacing(0.0),
      line_spacing(0.0),
      text_opacity(1.0),
      halo_opacity(1.0),
      text_transform(NONE),
      fill(0,0,0),
      halo_fill(0,0,0),
      halo_radius(0.0),
      ff_settings() {}
    std::string face_name;
    boost::optional<font_set> fontset;
    double text_size;
    double character_spacing;
    double line_spacing;
    double text_opacity;
    double halo_opacity;
    text_transform_e text_transform;
    color fill;
    color halo_fill;
    double halo_radius;
    font_feature_settings ff_settings;
};

struct evaluated_text_properties : noncopyable
{
    label_placement_e label_placement = POINT_PLACEMENT;
    double label_spacing = 0.0; // distance between repeated labels on a single geometry
    double label_position_tolerance = 0.0; // distance the label can be moved on the line to fit, if 0 the default is used
    bool avoid_edges = false;
    double margin = 0.0;
    double repeat_distance = 0.0;
    double minimum_distance = 0.0;
    double minimum_padding = 0.0;
    double minimum_path_length = 0.0;
    double max_char_angle_delta = 22.5 * M_PI/180.0;
    bool allow_overlap = false;
    bool largest_bbox_only = true; // Only consider geometry with largest bbox (polygons)
    text_upright_e upright = UPRIGHT_AUTO;

};

}

using evaluated_text_properties_ptr = std::unique_ptr<detail::evaluated_text_properties>;

enum directions_e
{
    NORTH,
    EAST,
    SOUTH,
    WEST,
    NORTHEAST,
    SOUTHEAST,
    NORTHWEST,
    SOUTHWEST,
    EXACT_POSITION
};

using fontset_map = std::map<std::string, font_set>;

struct MAPNIK_DECL format_properties
{
    format_properties();
    void from_xml(xml_node const& sym, fontset_map const& fontsets, bool is_shield);
    void to_xml(boost::property_tree::ptree & node, bool explicit_defaults,
                format_properties const& dfl) const;
    // collect expressions
    void add_expressions(expression_set & output) const;

    std::string face_name;
    boost::optional<font_set> fontset;
    // expressions
    symbolizer_base::value_type text_size;
    symbolizer_base::value_type character_spacing;
    symbolizer_base::value_type line_spacing; //Largest total height (fontsize+line_spacing) per line is chosen
    symbolizer_base::value_type text_opacity;
    symbolizer_base::value_type halo_opacity;
    symbolizer_base::value_type fill;
    symbolizer_base::value_type halo_fill;
    symbolizer_base::value_type halo_radius;
    symbolizer_base::value_type text_transform;
    symbolizer_base::value_type ff_settings;
};


// Properties for building the layout of a single text placement
struct MAPNIK_DECL text_layout_properties
{
    text_layout_properties();

    // Load all values from XML ptree.
    void from_xml(xml_node const &sym, fontset_map const& fontsets);
    // Save all values to XML ptree (but does not create a new parent node!).
    void to_xml(boost::property_tree::ptree & node, bool explicit_defaults,
                text_layout_properties const& dfl) const;

    // Get a list of all expressions used in any placement.
    // This function is used to collect attributes.
    void add_expressions(expression_set & output) const;

    // per layout expressions
    symbolizer_base::value_type dx;
    symbolizer_base::value_type dy;
    symbolizer_base::value_type orientation;
    symbolizer_base::value_type text_ratio;
    symbolizer_base::value_type wrap_width;
    symbolizer_base::value_type wrap_char;
    symbolizer_base::value_type wrap_before;
    symbolizer_base::value_type repeat_wrap_char;
    symbolizer_base::value_type rotate_displacement;
    symbolizer_base::value_type halign;
    symbolizer_base::value_type jalign;
    symbolizer_base::value_type valign;
    directions_e dir = EXACT_POSITION;
};

struct text_properties_expressions
{
    symbolizer_base::value_type label_placement = enumeration_wrapper(POINT_PLACEMENT);
    symbolizer_base::value_type label_spacing = 0.0;
    symbolizer_base::value_type label_position_tolerance = 0.0;
    symbolizer_base::value_type avoid_edges = false;
    symbolizer_base::value_type margin = 0.0;
    symbolizer_base::value_type repeat_distance = 0.0;
    symbolizer_base::value_type minimum_distance = 0.0;
    symbolizer_base::value_type minimum_padding = 0.0;
    symbolizer_base::value_type minimum_path_length = 0.0;
    symbolizer_base::value_type max_char_angle_delta = 22.5;
    symbolizer_base::value_type allow_overlap = false;
    symbolizer_base::value_type largest_bbox_only = true;
    symbolizer_base::value_type upright = enumeration_wrapper(UPRIGHT_AUTO);
};

// Contains all text symbolizer properties which are not directly related to text formatting and layout.
struct MAPNIK_DECL text_symbolizer_properties
{
    text_symbolizer_properties();
    // Load only placement related values from XML ptree.
    void text_properties_from_xml(xml_node const& node);
    // Load all values from XML ptree.
    void from_xml(xml_node const& node, fontset_map const& fontsets, bool is_shield);
    // Save all values to XML ptree (but does not create a new parent node!).
    void to_xml(boost::property_tree::ptree & node, bool explicit_defaults,
                text_symbolizer_properties const& dfl) const;

    // Takes a feature and produces formatted text as output.
    // The output object has to be created by the caller and passed in for thread safety.
    void process(text_layout & output, feature_impl const& feature, attributes const& vars) const;
    evaluated_text_properties_ptr evaluate_text_properties(feature_impl const& feature, attributes const& attrs) const;
    // Sets new format tree.
    void set_format_tree(formatting::node_ptr tree);
    // Get a list of all expressions used in any placement.
    // This function is used to collect attributes.
    void add_expressions(expression_set & output) const;

    // Per symbolizer options
    // Expressions
    text_properties_expressions expressions;
    // Default values for text layouts
    text_layout_properties layout_defaults;
    // Default values for format_properties.
    format_properties format_defaults;

private:
    // A tree of formatting::nodes which contain text and formatting information.
    formatting::node_ptr tree_;
};

} //ns mapnik

#endif // MAPNIK_TEXT_PROPERTIES_HPP
