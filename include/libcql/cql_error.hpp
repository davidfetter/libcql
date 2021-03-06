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

#ifndef CQL_ERROR_H_
#define CQL_ERROR_H_

#include <string>
#include "libcql/cql.hpp"

namespace cql {

    struct cql_error_t
    {
        bool        cassandra;
        bool        transport;
        bool        library;
        int         code;
        std::string message;

        cql_error_t() :
            cassandra(false),
            transport(false),
            library(false),
            code(0)
        {}

        inline bool
        is_err()
        {
            return cassandra || transport || library;
        }
    };
} // namespace cql

#endif // CQL_ERROR_H_
