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

#include <iomanip>
#include <iostream>
#include <sstream>
#include "serialization.hpp"
#include "util.hpp"

#include "cql_message_result.hpp"

cql::cql_message_result_t::cql_message_result_t() :
	_result_type(0)
{}

cql_int_t
cql::cql_message_result_t::result_type() const
{
	return _result_type;
}

uint8_t
cql::cql_message_result_t::opcode() const
{
	return CQL_OPCODE_RESULT;
}

cql_int_t
cql::cql_message_result_t::size() const
{
	std::stringstream ss(std::stringstream::out);
	write(ss);
	return ss.str().size();
}

std::string
cql::cql_message_result_t::str() const
{
	std::stringstream output;
	output << std::setfill('0');
	output << std::string("RESULT 0x") << std::setw(2) << cql::internal::hex(_result_type);
	output << " " << _metadata.str();
	return output.str();
}

std::istream&
cql::cql_message_result_t::read(std::istream& input)
{
	_keyspace_name.clear();
	_row_count = 0;

    cql::internal::decode_int(input, _result_type);

	switch(_result_type) {

	case CQL_RESULT_ROWS:
        _metadata.read(input);
        cql::internal::decode_int(input, _row_count);
		break;

	case CQL_RESULT_SET_KEYSPACE:
		cql::internal::decode_string(input, _keyspace_name);
		break;

	case CQL_RESULT_VOID:
		break;
	}

	return input;
}

std::ostream&
cql::cql_message_result_t::write(std::ostream& output) const
{
	int32_t result_type = htonl(_result_type);
	output.write(reinterpret_cast<char*>(&result_type), sizeof(result_type));
	return output;
}