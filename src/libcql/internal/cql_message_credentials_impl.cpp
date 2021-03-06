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
  along with this program.      If not, see <http://www.gnu.org/licenses/>.
*/

#include <map>
#include <sstream>

#include <boost/version.hpp>
#include <boost/foreach.hpp>
#if BOOST_VERSION >= 104300
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>
#else
#include <algorithm>
#include <ext/functional>
#endif
#include <boost/algorithm/string/join.hpp>

#include "libcql/cql_vector_stream.hpp"
#include "libcql/cql_serialization.hpp"
#include "libcql/internal/cql_defines.hpp"

#include "libcql/internal/cql_message_credentials_impl.hpp"

cql::cql_message_credentials_impl_t::cql_message_credentials_impl_t() :
    _buffer(new std::vector<cql_byte_t>(0))
{}

cql::cql_message_credentials_impl_t::cql_message_credentials_impl_t(size_t size) :
    _buffer(new std::vector<cql_byte_t>(size))
{}

cql::cql_message_buffer_t
cql::cql_message_credentials_impl_t::buffer()
{
    return _buffer;
}

void
cql::cql_message_credentials_impl_t::credentials(const std::map<std::string, std::string>& c)
{
    _credentials = c;
}

const std::map<std::string, std::string>&
cql::cql_message_credentials_impl_t::credentials() const
{
    return _credentials;
}

cql::cql_opcode_enum
cql::cql_message_credentials_impl_t::opcode() const
{
    return CQL_OPCODE_CREDENTIALS;
}

cql::cql_int_t
cql::cql_message_credentials_impl_t::size() const
{
    return _buffer->size();
}

std::string
cql::cql_message_credentials_impl_t::str() const
{
    std::list<std::string> keys;
#if BOOST_VERSION >= 104300
    boost::copy(_credentials | boost::adaptors::map_keys, std::back_inserter(keys));
#else
    std::transform(_credentials.begin(), _credentials.end(), std::back_inserter(keys), __gnu_cxx::select1st<credentials_map_t::value_type>());
#endif

    std::stringstream output;
    output << "[" << boost::algorithm::join(keys, ", ") << "]";
    return output.str();
}

bool
cql::cql_message_credentials_impl_t::consume(cql::cql_error_t* err)
{
    cql::vector_stream_t buffer(*_buffer);
    std::istream stream(&buffer);

    cql::decode_string_map(stream, _credentials);
    return true;
}

bool
cql::cql_message_credentials_impl_t::prepare(cql::cql_error_t* err)
{
    size_t size = 0;
    BOOST_FOREACH(const credentials_map_t::value_type& pair, _credentials) {
        size += pair.first.size();
        size += pair.second.size();
        size += 2;
    }
    _buffer->resize(size);

    cql::vector_stream_t buffer(*_buffer);
    std::ostream stream(&buffer);

    cql::encode_string_map(stream, _credentials);
    return true;
}
