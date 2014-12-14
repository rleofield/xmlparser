



#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include <boost/lexical_cast.hpp>

#include <locale>

#include "tLog_Category_default.h"

#include "xml_interface.h"
#include "rList.h"
#include "wList.h"

#include "xml_document.h"
#include "xml_text.h"
#include "xml_comment.h"

#include "xml_locator.h"

#include "xml_exception.h"
#include "xml_utl.h"

#include "stringhelper.h"
#include "alloccheck.h"


using std::vector;
using std::string;
using std::cout;
using std::endl;

namespace txml {




   class tXmlInterfaceImpl {

   public:
      tXmlInterfaceImpl();
      ~tXmlInterfaceImpl();

      void create_root( string const& name, string const& root );

      string getString( string const& key )const;
      int get_int( string const& key )const;
      double getDouble( string const& key )const;

      string get_string( string const& key, string default_ );
      int get_int( string const& key, int default_ );
      double getDoubleAt( string const& key, double default_ );

      void setStringAt( string const& key, string const& value );
      void setIntAt( string const& key, int value );
      void setDoubleAt( string const& key, double value );

      void writeString( string const& key, string const& value );
      void writeInt( string const& key, int value );
      void writeDouble( string const& key, double value );

      void setAttributeAt( string const& key, string const& value );
      void setIntAttributeAt( string const& key, int value );
      void setDoubleAttributeAt( string const& key, double value );

      void setComment( string const& key, string const& comment );

      void parse( string const& fn );
      void rebuild( xml_document const& doc );
      void rebuild();

      void save( string const& fn = string() );

      void printKeys( string const& fn )const;
      std::vector<string> Keys()const;

      void printKeysAndValues( string const& fn, size_t w = 60 )const;
      void printXmlGenerator( string const& fn_keys, string const& fn_out )const;
      void create( string const& key, string const& value );
      xml_document const* document()const {
         return &_doc;
      }


   private:
      string _filename;
      string root_name;
      xml_document _doc;
      bool _isparsed;
      mutable std::vector<keyentries> collected_keys;

      tXmlInterfaceImpl( tXmlInterfaceImpl const& );
      void operator=( tXmlInterfaceImpl const& );

      void collect_element_keys( xml_node const* node )const;
      void collect_attribute_keys( xml_node const* element )const;

      string getAt( string const& key )const;
      string getAt( string const& key, string default_ );
      void setAt( string const& key, string const& value );

      string addNotFoundNodeWithDefaultValue( xml_node* n,  keyentries remainder, string default_ );
      xml_node* findSmallestInsertPointForNewNode( xml_locator& p );




   };




   tXmlInterfaceImpl::tXmlInterfaceImpl(): _filename(), root_name(), _doc(), _isparsed( false ), collected_keys() {}


   tXmlInterfaceImpl::~tXmlInterfaceImpl() {
      //doc.pointers.clear();
   }

   void tXmlInterfaceImpl::rebuild( xml_document const& doc ) {

      _isparsed = false;
      string l;
      doc.serialize( l );
      collected_keys.clear();
      bool loadOkay = _doc.parse_begin( l );

      if( !loadOkay ) {
         throw xml_exception( tlog_lfm_,
                              eException::parsing_file, "Couldn't parse doc." );
      }

      collect_element_keys( &_doc );
      _isparsed = true;
   }


   void tXmlInterfaceImpl::rebuild() {
      rebuild( _doc );
   }

   namespace {
      void loadFile( string&  l, string const& name ) {
         try {
            rlf_txtrw::t_text_read_string()( name, l );
         } catch( rlf_txtrw::bad_text_read& ex ) {
            string msg = string( "error reading XML file: " ) + ex.what();
            throw xml_exception( tlog_lfm_,
                                 eException::reading_file, msg );
         }
      }
   }


   void tXmlInterfaceImpl::create_root( string const& name, string const& root ) {
      root_name = root;
      std::vector<string> text;
      text.push_back( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" );
      text.push_back( "<"  + root + ">" );
      text.push_back( "</" + root + ">" );
      bool overwrite = true;
      rlf_txtrw::t_write_ascii()( name, text, overwrite );
   }

   void tXmlInterfaceImpl::parse( string const& fn ) {
      string s = to_string( eException::document_empty ) ;

      if( fn.size() == 0 ) {
         return;
      }

      if( fn != _filename ) {
         _filename = fn;
         //_doc.clear();
         collected_keys.clear();

         _isparsed = false;
         string  l;
         loadFile( l, _filename );
         bool ok = _doc.parse_begin( l );

         if( !ok ) {
            throw xml_exception( tlog_lfm_,
                                 eException::parsing_file, "Couldn't parse file: '" + _filename + "'" );
         }

         collect_element_keys( &_doc );
         _isparsed = true;
      }

   }

   string tXmlInterfaceImpl::getAt( string const& key )const {
      if( !_isparsed ) {
         string msg = "no parsed document found";
         throw xml_exception( tlog_lfm_,
                              eException::no_parsed_document_found, msg );
      }

      xml_locator p( key );
      _doc.accept( &p );
      string value = p.value();
      string attr = p.attr();

      if( !attr.empty() ) {
         return attr;
      }

      return value;
   }

   string tXmlInterfaceImpl::addNotFoundNodeWithDefaultValue( xml_node* node,  keyentries remainder, string default_ ) {


      size_t size = remainder.size();
      xml_element* nnbase = nullptr;
      xml_element* first = nullptr;

      for( size_t i = 0; i < size; i++ ) {

         keyentry key_entry = remainder[i];
         string vs = key_entry.Element();
         xml_element* element = xml_element::create( vs );

         if( i == size - 1 ) {
            element->link_end_child( xml_text::create( default_ ) );
         }

         if( i == 0 ) {
            first = element;
         }

         if( nnbase != nullptr ) {
            nnbase->link_end_child( element );
         }

         nnbase = element;

      }

      node->link_end_child( first );

      return default_;

   }




   xml_node* tXmlInterfaceImpl::findSmallestInsertPointForNewNode( xml_locator& p ) {

      keyentry key_entry1 = p.lookupkeys.last();

      if( p.lookupkeys.size() < 2 && key_entry1.Element() != root_name ) {
         // first entry in key list is wrong == root element is not found
         return nullptr;
      }

      while( !p.accepted() ) {
         keyentry& key_entry = p.lookupkeys.last();

         if( key_entry.is_attr() ) {
            key_entry.removeAttr();
            // suche elem ohne attr.
            _doc.accept( & p );
         } else {
            p.remainder.insert_front( p.lookupkeys.last() );
            keyentries& kel = p.lookupkeys;
            kel.remove_last();
            // suche 2.
            _doc.accept( & p );

            if( p.lookupkeys.size() == 0 ) {
               return nullptr;
            }
         }
      }

      return p.elementfound();



   }

   string tXmlInterfaceImpl::getAt( string const& key, string default_ ) {
      if( default_ == "EventsGeneration" ) {
         cout << default_ << endl;

      }

      if( !_isparsed ) {
         string msg = "no parsed document found";
         throw xml_exception( tlog_lfm_,
                              eException::no_parsed_document_found, msg );
      }

      xml_locator locator( key );

      _doc.accept( &locator );
      string v ;

      string value = locator.value();
      string attr = locator.attr();

      if( locator.accepted() ) {
         if( !attr.empty() ) {
            return attr;
         }

         return value;
      }



      keyentry key_entry = locator.lookupkeys.last();

      xml_node* node = findSmallestInsertPointForNewNode( locator );

      if( node == nullptr ) {
         xml_exception ex( tlog_lfm_,
                           eException::cannot_insert_root_as_default,
                           msg_cannot_insert_root_as_default
                           + ", missingKey = " + key );
         throw ex;

      }

      if( key_entry.is_attr() ) {
         xml_element* elem = dynamic_cast<xml_element*>( node );
         elem->setAttribute( key_entry.attr(), default_ );
      } else {
         v = addNotFoundNodeWithDefaultValue( node, locator.remainder, default_ );
      }

      // rebuild internal structure

      rebuild();
      xml_locator locator2( key );
      _doc.accept( &locator2 );
      value = locator2.value();
      attr = locator2.attr();



      if( !attr.empty() ) {
         return attr;
      }

      return value;
   }


   string tXmlInterfaceImpl::getString( string const& key )const {
      return getAt( key );
   }

   int tXmlInterfaceImpl::get_int( string const& key )const {
      string s = getAt( key );
      return xmlinterface::to_int( s );
   }
   double tXmlInterfaceImpl::getDouble( string const& key )const {
      string s = getAt( key );
      double a = xmlinterface::to_double( s );
      return a;
   }
   // with defaults

   string tXmlInterfaceImpl::get_string( string const& key, string default_ ) {
      string s = getAt( key, default_ );
      return s;
   }


   int tXmlInterfaceImpl::get_int( string const& key, int default_ ) {
      string sdefault_ = std::to_string( default_ );
      string sf = get_string( key, sdefault_ );
      return xmlinterface::to_int( sf );
   }


   double tXmlInterfaceImpl::getDoubleAt( string const& key, double default_ ) {
      string sdefault_ = std::to_string( default_ );
      string sd = get_string( key, sdefault_ );
      double a = xmlinterface::to_double( sd );
      return a;
   }


   void tXmlInterfaceImpl::setAt( string const& key, string const& value ) {  // key points to an element
      if( !_isparsed ) {
         string msg = "no parsed document found";
         throw xml_exception( tlog_lfm_,
                              eException::no_parsed_document_found,  msg );
      }

      xml_locator p( key );

      if( p.isAttr() ) {

         throw xml_exception( tlog_lfm_,
                              eException::key_points_not_to_an_element, msg_key_points_not_to_an_element + ": '" + key + "'" );
      }

      _doc.accept( &p );

      if( p.accepted() ) {
         xml_element* e = p.elementfound();
         e->setText( value );
         return;
      }

      throw xml_exception( tlog_lfm_,
                           eException::key_not_found, "key not found: '" + key + "'" );
   }


   void tXmlInterfaceImpl::setStringAt( string const& key, string const& value ) {  // key points to an element
      setAt( key, value );
   }

   void tXmlInterfaceImpl::setIntAt( string const& key, int value ) {  // key points to an element
      string s = rlf_hstring::toString( value );
      setStringAt( key, s );
   }

   void tXmlInterfaceImpl::setDoubleAt( string const& key, double value ) { // key points to an element
      string s = rlf_hstring::toString( value );
      setStringAt( key, s );
   }



   void tXmlInterfaceImpl::setAttributeAt( string const& key, string const& value ) {  // key points to an attribute
      if( !_isparsed ) {
         string msg = "no parsed document found";
         throw xml_exception( tlog_lfm_,
                              eException::no_parsed_document_found, msg );
      }

      xml_locator p( key );

      if( !p.isAttr() ) {
         throw xml_exception( tlog_lfm_,
                              eException::key_points_not_to_an_attribute, "key didn't point to an attribute: '" + key + "'" );
      }

      _doc.accept( &p );

      if( p.accepted() ) {
         p.setElementAttr( value );
         save();
         return;
      }

      throw xml_exception( tlog_lfm_,
                           eException::key_not_found, "key not found: '" + key + "'" );
   }

   void tXmlInterfaceImpl::setIntAttributeAt( string const& key, int value ) {  // key points to an attribute
      setAttributeAt( key, rlf_hstring::toString( value ) );
   }

   void tXmlInterfaceImpl::setDoubleAttributeAt( string const& key, double value ) {  // key points to an attribute
      setAttributeAt( key, rlf_hstring::toString( value ) );
   }


   void tXmlInterfaceImpl::setComment( string const& key, string const& value ) {  // key points to an element
      if( !_isparsed ) {
         string msg = "no parsed document found";
         throw xml_exception( tlog_lfm_,
                              eException::no_parsed_document_found, msg );
      }

      // locate insert point in tree
      xml_locator p( key );

      if( p.isAttr() ) {
         throw xml_exception( tlog_lfm_,
                              eException::key_points_not_to_an_element, "key didn't point to an element: '" + key + "'" );
      }

      _doc.accept( &p );

      if( p.accepted() ) {
         xml_element* elementfound = p.elementfound();
         xml_comment* comment = xml_comment::create( tlog_lfm_ ) ;
         comment->value( value );

         keyentries const& lookuppath = elementfound->lookuppath();
         xml_node const* parent   = lookuppath.parentOfLast();

         if( parent != nullptr ) {
            const_cast<xml_node*>( parent )->insert_comment_before( elementfound, comment );
         }

         return;
      }

      throw xml_exception( tlog_lfm_,
                           eException::key_not_found, "key not found: '" + key + "'" );
   }

   void tXmlInterfaceImpl::save( string const& fn ) {
      string f = fn;

      if( f.empty() ) {
         f = _filename;
      }

      _doc.save( f );
   }



   void tXmlInterfaceImpl::collect_element_keys( xml_node const* node ) const {
      if( node == nullptr ) {
         return;
      }

      xml_element const* element = dynamic_cast<xml_element const*>( node );

      if( element != nullptr ) {
         string lp = element->lookuppath().to_string();
         collected_keys.push_back( element->lookuppath() );
         collect_attribute_keys( element );
      }

      xml_node const* child;

      for( child = node->firstChild(); child != nullptr; child = child->next() ) {
         collect_element_keys( child );
      }
   }

   void tXmlInterfaceImpl::collect_attribute_keys( xml_node const* node )const {
      auto const* element = dynamic_cast<xml_element const*>( node );

      if( element == nullptr ) {
         return ;
      }

      auto const& v = element->Attributes();

      for( auto const & k : v ) {
         collected_keys.push_back( k.path );
      }
   }



   void tXmlInterfaceImpl::printKeys( string const& fn )const {
      auto v = Keys();
      std::vector<string> l( v.begin(), v.end() );
      bool overwrite = true;
      rlf_txtrw::t_write_ascii()( fn, l, overwrite );
   }


   std::vector<string> tXmlInterfaceImpl::Keys()const {
      std::vector<string> v;

      for( auto const & keyentry : collected_keys ) {
         string temp = keyentry.to_string();
         v.push_back( temp );
      }

      return v;
   }



   void tXmlInterfaceImpl::printKeysAndValues( string const& fn, size_t w )const {
      std::vector<string> l;

      for( auto const & keyentries : collected_keys ) {
         string temp = keyentries.to_string();
         keyentry const& pe = keyentries.last();
         string val = pe.toValue();

         if( ! val.empty() ) {
            string txt = "-->   element: ";

            if( pe.is_attr() ) {
               txt =     "--> attribute: ";
            }

            string space = "  ";

            while( ( space.size() + txt.size() + temp.size() ) < w ) {
               space += "-";
            }

            temp += space + txt + "'" + val + "'";
         }

         l.push_back( temp );

      }

      bool overwrite = true;
      rlf_txtrw::t_write_ascii()( fn, l, overwrite );
   }

   namespace {
      struct add {
         string s;
         string const& sep;
         add( string const& sep_ ): s(), sep( sep_ ) {}
         void operator()( string const& str ) {
            if( s.empty() ) {
               s += str;
               return;
            }

            s += sep;
            s += str;
         }
      };

      string merge( vector<string>  const& v, string const& sep ) {
         if( v.empty() ) {
            return string();
         }

         if( v.size() == 1 ) {
            return v[0];
         }

         return for_each( v.begin(), v.end(), add( sep ) ).s;
      }
      string replace_line_marker_with_underscore( string const& n ) {
         string line = n;
         line = rlf_hstring::replace_all( line, ".", "_" );
         line = rlf_hstring::replace_all( line, "-", "_" );
         line = rlf_hstring::replace_all( line, ":", "__" );
         line = rlf_hstring::replace_all( line, "[", "__" );
         line = rlf_hstring::replace_all( line, "]", "" );
         return line;

      }
   }



   void tXmlInterfaceImpl::printXmlGenerator( string const& filename, string const& fn_out )const {
      struct lv {
         string line;
         string val;
         lv( string l, string v ): line( l ), val( v ) {}
      };

      vector<string> l;
      string root1; // == firstline
      static string space = "  // ";

      vector<string> root_attr;
      vector<lv> lines;

      for( auto const & key : collected_keys ) {

         string line = key.to_string();

         if( root1.size() == 0 ) {
            root1 = line;
            continue;
         }

         string val = key.last().toValue();
         string temp_line = line;

         if( line.find( root1 + ":" ) != string::npos ) {
            string attr = rlf_hstring::replace_all( temp_line, root1 + ":", "" ) ;
            attr += "=\"" + val + "\"";
            root_attr.push_back( attr );
            continue;
         }

         line = replace_line_marker_with_underscore( line );
         string temp = "const string " + line + " = \"";
         temp += temp_line + "\";";

         string txt = "  element:   ";

         if( key.last().is_attr() ) {
            txt =     "  attribute: ";
         }

         string final_line = temp + space + txt + " = '" + val + "'";
         l.push_back( final_line );
         lines.push_back( lv( temp_line, val ) );
      }


      l.push_back( "" );
      l.push_back( "" );



      string root_attr2 = merge( root_attr, " " );

      std::vector<string> text;
      text.push_back( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" );
      text.push_back( "<" + root1 + " " + root_attr2 + "/>" );
      text.push_back( "" );

      bool overwrite = true;
      rlf_txtrw::t_write_ascii()( fn_out, text, overwrite );

      l.push_back( "xmlinterface::tXmlInterface instance;" );
      l.push_back( "instance.parse( xmlTestOut );" );

      root1 = string();

      for( auto const & val : lines ) {
         string final_line = "instance.create( \"" + val.line + "\", \"" + val.val + "\" );";
         l.push_back( final_line );
      }

      l.push_back( "instance.save();" );
      rlf_txtrw::t_write_ascii()( filename, l, overwrite );
   }

   void tXmlInterfaceImpl::create( string const& key, string const& value ) {
      get_string( key, value );
   }



   void showAllocList() {
      int i = alloccheck::alloc_list_size();
      LOGT_INFO( " end of program, number of not deleted pointers: " + rlf_hstring::toString( i ) );

   }
   std::string allocliststring() {
      return alloccheck::allocliststring();

   }



   int str_to_int( string const& s )  {
      return xmlinterface::to_int( s ) ;
   }
   double str_to_double( string const& s )  {
      return xmlinterface::to_double( s );
   }

   string int_to_string( int val )  {
      return rlf_hstring::toString( val );
   }


   string double_to_string( double val )  {
      return rlf_hstring::trim( rlf_hstring::toString( val ) );
   }


   std::vector<string> split( string const& l, string const& pat ) {
      return rlf_hstring::split( l, pat );
   }
   string replace_all( string const& ins, const string& pattern, const string& replace ) {
      return rlf_hstring::replace_all( ins, pattern, replace );
   }
   size_t index( string const& s, string const& pattern, size_t pos ) {
      return rlf_hstring::index( s, pattern, pos );
   }
   string trim( string const& str, char ch ) {
      return rlf_hstring::trim( str, ch );
   }


   string fillup( string const& in, char ch, size_t n ) {
      return rlf_hstring::fillup( in, ch, n );
   }
   string to_string( double val ) {
      return rlf_hstring::toString( val );
   }
   string to_string( int val ) {
      return rlf_hstring::toString( val );
   }
   //   int to_int( string const& s ) {
   //      return rlf_hstring::to_int( s );
   //   }
   //   // double to_double( string const& s, std::locale l ) {
   //   // return strings::to_double( s, l );
   //   //}
   //   double to_double( string const& s ) {
   //      return rlf_hstring::to_double( s );
   //   }

}


// forward to tXmlInterfaceImpl

namespace xmlinterface {

   int to_int( string const& s )  {
      try {
         return boost::lexical_cast<int>( s );
      } catch( boost::bad_lexical_cast& e ) {
         string msg = e.what();
         throw txml::xml_exception( tlog_lfm_,
                                    txml::eException::bad_lexical_cast, "msg: " + msg );
      }

   }

   double to_double( string const& s )  {
      try {
         double b = boost::lexical_cast<double>( rlf_hstring::trim( s ) );
         return b;
      } catch( boost::bad_lexical_cast& e ) {
         string msg = e.what();
         throw txml::xml_exception( tlog_lfm_,
                                    txml::eException::bad_lexical_cast, "string : " + s + ", msg: " + msg );
      }
   }



   tXmlInterface::tXmlInterface(): impl( new txml::tXmlInterfaceImpl() ) {}


   tXmlInterface::~tXmlInterface() {
      delete impl;
   }

   void tXmlInterface::rebuild( txml::xml_document const& doc ) {
      impl->rebuild( doc );
   }


   void tXmlInterface::parse( string const& fn ) {
      impl->parse( fn );
   }


   void tXmlInterface::create_root( string const& name, string const& root ) {
      impl->create_root( name, root );
   }



   string tXmlInterface::get_string( string const& key )const {
      return impl->getString( key );
   }


   int tXmlInterface::get_int( string const& key )const {
      return impl->get_int( key );
   }

   double tXmlInterface::get_double( string const& key )const  {
      return impl->getDouble( key );
   }

   // with defaults
   string tXmlInterface::get_string( string const& key, string default_ )  {
      return impl->get_string( key, default_ );
   }


   int tXmlInterface::get_int( string const& key, int default_ )  {
      return impl->get_int( key, default_ );
   }

   double tXmlInterface::get_double( string const& key, double default_ )  {
      return impl->getDoubleAt( key, default_ );
   }

   void tXmlInterface::set_string( string const& key, string const& value ) {  // key points to an element
      impl->setStringAt( key, value );
   }


   void tXmlInterface::set_int( string const& key, int value ) {  // key points to an element
      impl->setIntAt( key, value );
   }

   void tXmlInterface::set_double( string const& key, double value ) { // key points to an element
      impl->setDoubleAt( key, value );
   }


   void tXmlInterface::set_attribute( string const& key, string const& value ) {  // key points to an attribute
      impl->setAttributeAt( key, value );
   }

   void tXmlInterface::set_int_attribute( string const& key, int value ) {  // key points to an attribute
      impl->setIntAttributeAt( key, value );
   }

   void tXmlInterface::set_double_attribute( string const& key, double value ) {  // key points to an attribute
      impl->setDoubleAttributeAt( key, value );
   }


   void tXmlInterface::setComment( string const& key, string const& value ) {  // key points to an element
      impl->setComment( key, value );
   }



   void tXmlInterface::save( string const& fn ) {
      impl->save( fn );
   }


   void tXmlInterface::save() {
      impl->save();
   }





   void tXmlInterface::print_keys( string const& fn )const {
      impl->printKeys( fn );
   }


   std::vector<string> tXmlInterface::Keys()const {
      return impl->Keys();
   }

   void tXmlInterface::print_keys_and_values( string const& fn, size_t w )const {
      impl->printKeysAndValues( fn, w );

   }


   void tXmlInterface::print_creator( string const& fn_keys, string const& fn_out )const {
      impl->printXmlGenerator( fn_keys, fn_out );
   }
   void tXmlInterface::create( string const& key, string const& value ) {
      cout << "process: " << key << "::" << value << endl;
      impl->create( key, value );
   }


   txml::xml_document const* tXmlInterface::document()const {
      return impl->document();
   }

   void show_alloc_list() {
      txml::showAllocList();
   }
   std::string allocliststring() {
      return txml::allocliststring();
   }



   string int_to_string( int val )  {
      return  txml::int_to_string( val );
   }

   string double_to_string( double val )  {
      return  txml::double_to_string( val );
   }

   std::vector<string> split( string const& l, string const& pat ) {
      return  txml::split( l, pat );
   }
   string replace_all( string const& ins, const string& pattern, const string& replace ) {
      return  txml::replace_all( ins, pattern, replace );
   }
   size_t index( string const& s, string const& pattern, size_t pos ) {
      return txml::index( s, pattern, pos );
   }
   string trim( string const& str, char ch ) {
      return rlf_hstring::trim( str, ch );
   }


   string fillup( string const& in, char ch, size_t n ) {
      return rlf_hstring::fillup( in, ch, n );
   }
   string to_string( double val ) {
      return rlf_hstring::toString( val );
   }
   string to_string( int val ) {
      return rlf_hstring::toString( val );
   }
   //   int to_int( string const& s ) {
   //      return rlf_hstring::to_int( s );
   //   }
   //   // double to_double( string const& s, std::locale l ) {
   //   // return strings::to_double( s, l );
   //   //}
   //   double to_double( string const& s ) {
   //      return rlf_hstring::to_double( s );
   //   }

}






//EOF
