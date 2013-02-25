//  grammar-status.hpp -- component rule declarations
//  Copyright (C) 2012  SEIKO EPSON CORPORATION
//
//  License: GPL-3.0+
//  Author : AVASYS CORPORATION
//
//  This file is part of the 'Utsushi' package.
//  This package is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License or, at
//  your option, any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//  You ought to have received a copy of the GNU General Public License
//  along with this package.  If not, see <http://www.gnu.org/licenses/>.

#ifndef drivers_esci_grammar_status_hpp_
#define drivers_esci_grammar_status_hpp_

//! \copydoc grammar.hpp

#include <boost/operators.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/include/qi_rule.hpp>

#include "code-token.hpp"
#include "grammar-formats.hpp"

namespace utsushi {
namespace _drv_ {
namespace esci {

//! Store decoded replies to status requests
/*! Replies to status requests contain information on one (or more) of
 *  the detected medium size, error conditions, focus state as well as
 *  push_button state.
 */
struct hardware_status
  : private boost::equality_comparable< hardware_status >
{
  bool operator== (const hardware_status& rhs) const;

  void clear ();

  /*! Detected medium size and error conditions include information on
   *  the part of the device to which the result applies as well as
   *  what exactly was detected.
   */
  struct result
    : private boost::equality_comparable< result >
  {
    result (const quad& part = quad (),
            const quad& what = quad ());

    bool operator== (const result& rhs) const;

    quad part_;
    quad what_;
  };

  boost::optional< result > medium;
  boost::optional< result > error;

  /*! A special value of \c esci_non_int indicates that the device is
   *  calibrating.
   */
  boost::optional< integer > focus;
  boost::optional< integer > push_button;
};

namespace decoding {

namespace qi = boost::spirit::qi;

template< typename Iterator >
class basic_grammar_status
  : virtual protected basic_grammar_formats< Iterator >
{
public:
  basic_grammar_status ();

  //! Decodes a payload associated with the reply to a status request
  /*! \sa code_token::status
   */
  bool
  hardware_status_(Iterator& head, const Iterator& tail,
                   hardware_status& stat)
  {
    return this->parse (head, tail, hardware_status_rule_, stat);
  }

protected:
  qi::rule< Iterator, hardware_status () > hardware_status_rule_;

  qi::rule< Iterator, hardware_status::result () > stat_psz_rule_;
  qi::rule< Iterator, hardware_status::result () > stat_err_rule_;
  qi::rule< Iterator, integer () > stat_fcs_rule_;

  qi::rule< Iterator, quad () > stat_psz_part_token_;
  qi::rule< Iterator, quad () > stat_psz_size_token_;
  qi::rule< Iterator, quad () > stat_err_part_token_;
  qi::rule< Iterator, quad () > stat_err_what_token_;
};

extern template class basic_grammar_status< default_iterator_type >;

}       // namespace decoding

}       // namespace esci
}       // namespace _drv_
}       // namespace utsushi

#endif  /* drivers_esci_grammar_status_hpp_ */