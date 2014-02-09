/* --------------------------------------------------------------------------
Copyright 2012 by Richard Albrecht
richard.albrecht@rleofield.de
www.rleofield.de

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
------------------------------------------------------------------------------
*/

#ifndef ti_tFilename_H
#define ti_tFilename_H

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cerrno>

#include "boost_cstdint.h"
#include "boost/filesystem/path.hpp"
using std::string;

namespace rlf_filefn {


   //   path  root_name() const;
   //    path  root_directory() const;
   //    path  root_path() const;
   //    path  relative_path() const;
   //    path  parent_path() const;
   //    path  filename() const;
   //    path  stem() const;
   //    path  extension() const;

   //    // query
   //    bool empty() const;
   //    bool has_root_name() const;
   //    bool has_root_directory() const;
   //    bool has_root_path() const;
   //    bool has_relative_path() const;
   //    bool has_parent_path() const;
   //    bool has_filename() const;
   //    bool has_stem() const;
   //    bool has_extension() const;
   //    bool is_absolute() const;
   //    bool is_relative() const;


   class fnimpl;
   class t_filename {

      friend class fnimpl;

      fnimpl* impl;


   public:
      t_filename();
      t_filename( string const& f );
      t_filename( const std::string& path_, const std::string& filebase_, const std::string& extension_ );
      virtual ~t_filename();
      t_filename( const t_filename& fn );

      t_filename& operator=( const t_filename& s );

      bool operator==( const t_filename& s )const;

      // base name of file, no path,
      void base( const std::string& f );


      // path of file name,
      void path( const std::string& f );


      // set file extension
      // chars after a last dot in filename
      void extension( const std::string& f );

      //
      std::string extension()  const;

      // set filename from path, name, extension
      void fullname( const std::string& p, const std::string& f, const std::string& e );

      // get file + '.' + extension
      std::string filename()const;

      // get 'path + file + extension'
      std::string fullname()const;

      // get 'drive + path', in unix 'path' only
      std::string path()const;

      // get filename, without extension !!
      std::string base() const;

      std::string key()const;

      // system functions
      bool file_exists()const;
      bool path_exists()const;
      bool is_folder()const;

   };


   t_filename splitpath( string const& in ) ;

   inline void operator+= ( std::vector<t_filename> &v, t_filename const& s ) {
      v.push_back( s );
   }

   inline void operator+=( std::vector<t_filename> &v, const std::vector<t_filename> & in ) {
      for( size_t i = 0; i < in.size(); i++ ) {
         v += in[i];
      }
   }


} // end of namespace rlf






#endif

//EOF

