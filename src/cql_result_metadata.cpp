/*
  Copyright (c) 2012 Matthew Stump

  This file is part of libcql.

  libcql is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  libcql is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.	If not, see <http://www.gnu.org/licenses/>.
*/

#include <sstream>
#include <boost/algorithm/string/join.hpp>

#include "serialization.hpp"
#include "cql_result_metadata.hpp"

std::string
column_name_to_str(cql::internal::cql_result_metadata_t::column_name_t n)
{
    return std::string("[") + n.get<0>() + ", " + n.get<1>() + ", " + n.get<2>() + "]";
}


cql::internal::cql_result_metadata_t::cql_result_metadata_t()
{}

std::string
cql::internal::cql_result_metadata_t::str() const
{
    std::list<std::string> columns;
    std::transform(_column_order.begin(), _column_order.end(), std::back_inserter(columns), column_name_to_str);

    std::stringstream output;
    output << "cql_result_metadata_t [";
    output << boost::algorithm::join(columns, ", ") << "]";
    return output.str();
}

std::istream&
cql::internal::cql_result_metadata_t::read(std::istream& input)
{
	cql::internal::decode_int(input, _flags);
	cql::internal::decode_int(input, _column_count);

	std::string keyspace_name;
	std::string table_name;

	if (_flags & CQL_RESULT_ROWS_FLAGS_GLOBAL_TABLES_SPEC)
	{
		cql::internal::decode_string(input, keyspace_name);
		cql::internal::decode_string(input, table_name);
	}

	for (int i = 0; i < _column_count; ++i)
	{
		if (! _flags & CQL_RESULT_ROWS_FLAGS_GLOBAL_TABLES_SPEC)
		{
			cql::internal::decode_string(input, keyspace_name);
			cql::internal::decode_string(input, table_name);
		}
		std::string column_name;
		cql::internal::decode_string(input, column_name);

        option_t option;
		cql::internal::decode_option(input, option.id, option.value);

        column_name_t name(keyspace_name, table_name, column_name);
        _column_map.insert(column_map_t::value_type(name, option));
        _column_order.push_back(name);
	}
    return input;
}

cql_int_t
cql::internal::cql_result_metadata_t::flags() const
{
	return _flags;
}

void
cql::internal::cql_result_metadata_t::flags(cql_int_t v)
{
	_flags = v;
}

cql_int_t
cql::internal::cql_result_metadata_t::column_count() const
{
    return _column_count;
}

void
cql::internal::cql_result_metadata_t::column_count(cql_int_t v)
{
    _column_count = v;
}

bool
cql::internal::cql_result_metadata_t::has_global_keyspace() const
{
    return !_global_keyspace_name.empty();
}

bool
cql::internal::cql_result_metadata_t::has_global_table() const
{
    return !_global_table_name.empty();
}

const std::string&
cql::internal::cql_result_metadata_t::global_keyspace() const
{
    return _global_keyspace_name;
}

void
cql::internal::cql_result_metadata_t::global_keyspace(const std::string& keyspace)
{
    _global_keyspace_name = keyspace;
}

const std::string&
cql::internal::cql_result_metadata_t::global_table() const
{
    return _global_table_name;
}

void
cql::internal::cql_result_metadata_t::global_table(const std::string& table)
{
    _global_table_name = table;
}

bool
cql::internal::cql_result_metadata_t::column_class(int i,
                                                   std::string& output) const
{
    if (i > _column_count || i < 0)
        return false;

    column_name_t name = _column_order[i];
    if (_column_map.find(name) != _column_map.end()) {
        output = _column_map.at(name).value;
        return true;
    }
    return false;
}

bool
cql::internal::cql_result_metadata_t::column_class(const std::string& column,
                                                   std::string& output) const
{
    if (_global_keyspace_name.empty() || _global_table_name.empty())
        return false;

    return column_class(_global_keyspace_name, _global_table_name, column, output);
}

bool
cql::internal::cql_result_metadata_t::column_class(const std::string& keyspace,
                                                   const std::string& table,
                                                   const std::string& column,
                                                   std::string& output) const
{
    column_name_t name(keyspace, table, column);
    if (_column_map.find(name) != _column_map.end()) {
        output = _column_map.at(name).value;
        return true;
    }
    return false;
}

bool
cql::internal::cql_result_metadata_t::column_type(int i,
                                                  cql_short_t& output) const
{
    if (i > _column_count || i < 0)
        return false;

    column_name_t name = _column_order[i];
    if (_column_map.find(name) != _column_map.end()) {
        output = _column_map.at(name).id;
        return true;
    }
    return false;
}

bool
cql::internal::cql_result_metadata_t::column_type(const std::string& column,
                                                  cql_short_t& output) const
{
    if (_global_keyspace_name.empty() || _global_table_name.empty())
        return false;

    return column_type(_global_keyspace_name, _global_table_name, column, output);
}

bool
cql::internal::cql_result_metadata_t::column_type(const std::string& keyspace,
                                                  const std::string& table,
                                                  const std::string& column,
                                                  cql_short_t& output) const
{
    column_name_t name(keyspace, table, column);
    if (_column_map.find(name) != _column_map.end()) {
        output = _column_map.at(name).id;
        return true;
    }
    return false;
}

bool
cql::internal::cql_result_metadata_t::exists(const std::string& column) const
{
    if (_global_keyspace_name.empty() || _global_table_name.empty())
        return false;

    return _column_map.find(
        column_name_t(_global_keyspace_name, _global_table_name, column))
        != _column_map.end();
}

bool
cql::internal::cql_result_metadata_t::exists(const std::string& keyspace,
                                             const std::string& table,
                                             const std::string& column) const
{
    return _column_map.find(
        column_name_t(keyspace, table, column))
        != _column_map.end();
}

const std::vector<cql::internal::cql_result_metadata_t::column_name_t>&
cql::internal::cql_result_metadata_t::column_names() const
{
    return _column_order;
}