/*
  Copyright (c) 2012 Matthew Stump

  This file is part of libcql.

  libcql is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  libcql is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CQL_RESULT_H_
#define CQL_RESULT_H_

#include <vector>
#include "libcql/cql.hpp"

namespace cql {

    class cql_list_t;
    class cql_map_t;
    class cql_set_t;

    class cql_result_t
    {

    public:
        virtual
        ~cql_result_t() {};

        virtual cql::cql_result_type_enum
        result_type() const = 0;

        virtual cql::cql_opcode_enum
        opcode() const = 0;

        virtual std::string
        str() const = 0;

        virtual size_t
        column_count() const = 0;

        virtual size_t
        row_count() const = 0;

        virtual const std::vector<cql::cql_byte_t>&
        query_id() const = 0;

        virtual bool
        next() = 0;

        virtual bool
        exists(const std::string& column) const = 0;

        virtual bool
        column_class(int i,
                     std::string& output) const = 0;

        virtual bool
        column_class(const std::string& column,
                     std::string& output) const = 0;

        virtual bool
        column_type(int i,
                    cql_column_type_enum& output) const = 0;

        virtual bool
        column_type(const std::string& column,
                    cql_column_type_enum& output) const = 0;

        virtual bool
        get_index(const std::string& column,
                  int& output) const = 0;

        virtual bool
        is_null(int i,
                bool& output) const = 0;

        virtual bool
        is_null(const std::string& column,
                bool& output) const = 0;

        virtual bool
        get_bool(int i,
                 bool& output) const = 0;

        virtual bool
        get_bool(const std::string& column,
                 bool& output) const = 0;

        virtual bool
        get_int(int i,
                cql_int_t& output) const = 0;

        virtual bool
        get_int(const std::string& column,
                cql_int_t& output) const = 0;

        virtual bool
        get_float(int i,
                  float& output) const = 0;

        virtual bool
        get_float(const std::string& column,
                  float& output) const = 0;

        virtual bool
        get_double(int i,
                   double& output) const = 0;

        virtual bool
        get_double(const std::string& column,
                   double& output) const = 0;

        virtual bool
        get_bigint(int i,
                   cql::cql_bigint_t& output) const = 0;

        virtual bool
        get_bigint(const std::string& column,
                   cql::cql_bigint_t& output) const = 0;

        virtual bool
        get_string(int i,
                   std::string& output) const = 0;

        virtual bool
        get_string(const std::string& column,
                   std::string& output) const = 0;

        virtual bool
        get_data(int i,
                 cql::cql_byte_t** output,
                 cql::cql_int_t& size) const = 0;

        virtual bool
        get_data(const std::string& column,
                 cql::cql_byte_t** output,
                 cql::cql_int_t& size) const = 0;

        virtual bool
        get_list(int i,
                 cql::cql_list_t** output) const = 0;

        virtual bool
        get_list(const std::string& column,
                 cql::cql_list_t** output) const = 0;

        virtual bool
        get_set(int i,
                cql::cql_set_t** output) const = 0;

        virtual bool
        get_set(const std::string& column,
                cql::cql_set_t** output) const = 0;

        virtual bool
        get_map(int i,
                cql::cql_map_t** output) const = 0;

        virtual bool
        get_map(const std::string& column,
                cql::cql_map_t** output) const = 0;


    };

} // namespace cql

#endif // CQL_RESULT_H_
