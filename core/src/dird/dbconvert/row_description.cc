/*
   BAREOS® - Backup Archiving REcovery Open Sourced

   Copyright (C) 2000-2012 Free Software Foundation Europe e.V.
   Copyright (C) 2011-2016 Planets Communications B.V.
   Copyright (C) 2020-2020 Bareos GmbH & Co. KG

   This program is Free Software; you can redistribute it and/or
   modify it under the terms of version three of the GNU Affero General Public
   License as published by the Free Software Foundation and included
   in the file LICENSE.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.
*/

#include "row_description.h"

RowDescription::RowDescription(const char* column_name_in,
                               const char* data_type_in,
                               const char* max_length_in)
    : column_name(column_name_in), data_type(data_type_in)
{
  std::string field;
  try {
    field = max_length_in ? max_length_in : "";
    if (!field.empty()) { character_maximum_length = std::stoul(field); }
  } catch (const std::exception& e) {
    std::string err{__FILE__};
    err += ":";
    err += std::to_string(__LINE__);
    err += ": Could not convert number ";
    err += "<";
    err += field;
    err += ">";
    throw std::runtime_error(err);
  }
}

static const char* no_conversion(const char* in) { return in; }

const std::map<std::string, std::function<const char*(const char*)>>
    RowDescriptionMysql::data_type_converter{
        {"bigint", no_conversion},   {"binary", no_conversion},
        {"blob", no_conversion},     {"char", no_conversion},
        {"datetime", no_conversion}, {"decimal", no_conversion},
        {"enum", no_conversion},     {"int", no_conversion},
        {"longblob", no_conversion}, {"smallint", no_conversion},
        {"tinyblob", no_conversion}, {"tinyint", no_conversion}};

RowDescriptionMysql::RowDescriptionMysql(const char* column_name_in,
                                         const char* data_type_in,
                                         const char* max_length_in)
    : RowDescription(column_name_in, data_type_in, max_length_in)
{
}

const std::map<std::string, std::function<const char*(const char*)>>
    RowDescriptionPostgresql::data_type_converter{
        {"bigint", no_conversion},
        {"bytea", no_conversion},
        {"character", no_conversion},
        {"integer", no_conversion},
        {"numeric", no_conversion},
        {"smallint", no_conversion},
        {"text", no_conversion},
        {"timestamp without time zone", no_conversion}};

RowDescriptionPostgresql::RowDescriptionPostgresql(const char* column_name_in,
                                                   const char* data_type_in,
                                                   const char* max_length_in)
    : RowDescription(column_name_in, data_type_in, max_length_in)
{
}