



#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include <boost/scoped_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <locale>

#include "tLog_Category_A.h"

#include "xml_interface.h"
#include "rList.h"
#include "wList.h"

#include "XmlDocument.h"
#include "XmlText.h"
#include "XmlComment.h"

#include "XmlLocator.h"

#include "XmlException.h"
#include "xml_utl.h"

#include "stringhelper.h"
#include "alloccheck.h"


using namespace std;

namespace txml {




   class tXmlInterfaceImpl {

   public:
      tXmlInterfaceImpl();
      ~tXmlInterfaceImpl();

      void create_root( std::string const& name, std::string const& root );

      std::string getString( std::string const& key )const;
      int get_int( std::string const& key )const;
      double getDouble( std::string const& key )const;

      std::string get_string( std::string const& key, std::string default_ );
      int get_int( std::string const& key, int default_ );
      double getDoubleAt( std::string const& key, double default_ );

      void setStringAt( std::string const& key, std::string const& value );
      void setIntAt( std::string const& key, int value );
      void setDoubleAt( std::string const& key, double value );

      void writeString( std::string const& key, std::string const& value );
      void writeInt( std::string const& key, int value );
      void writeDouble( std::string const& key, double value );

      void setAttributeAt( std::string const& key, std::string const& value );
      void setIntAttributeAt( std::string const& key, int value );
      void setDoubleAttributeAt( std::string const& key, double value );

      void setComment( std::string const& key, std::string const& comment );

      void parse( std::string const& fn );
      void parse( txml::XmlDocument const& doc );
      void parse();

      void save( std::string const& fn );
      void save();

      void printKeys( std::string const& fn )const;
      std::vector<std::string> Keys()const;

      void printKeysAndValues( std::string const& fn, size_t w = 60 )const;
      void printXmlGenerator( std::string const& fn_keys, std::string const& fn_out )const;
      void create( string const& key, string const& value );

   private:
      std::string _filename;
      string root_name;
      XmlDocument _doc;
      bool _isparsed;
      mutable std::vector<txml::keyentries> collected_keys;

      tXmlInterfaceImpl( tXmlInterfaceImpl const& );
      void operator=( tXmlInterfaceImpl const& );

      void collect_element_keys( XmlNode const* node )const;
      void collect_attribute_keys( XmlNode const* element )const;

      std::string getAt( std::string const& key )const;
      std::string getAt( std::string const& key, std::string default_ );
      void setAt( std::string const& key, std::string const& value );

      std::string addNotFoundNodeWithDefaultValue( txml::XmlNode* n,  txml::keyentries remainder, std::string default_ );
      XmlNode* findSmallestInsertPointForNewNode( txml::XmlLocator& p );


   };


   tXmlInterfaceImpl::tXmlInterfaceImpl(): _filename(), root_name(), _doc(), _isparsed( false ), collected_keys() {}


   tXmlInterfaceImpl::~tXmlInterfaceImpl() {}

   void tXmlInterfaceImpl::parse( txml::XmlDocument const& doc ) {

      _isparsed = false;
      list<string> l;
      doc.serialize( l );
      bool loadOkay = _doc.parseStart( l );

      if( !loadOkay ) {
         throw txml::XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   enum_parsing_file, "Couldn't parse doc." );
      }

      collect_element_keys( &_doc );
      _isparsed = true;
   }


   void tXmlInterfaceImpl::parse( ) {

      _isparsed = false;
      list<string> l;
      _doc.serialize( l );

      bool loadOkay = _doc.parseStart( l );

      if( !loadOkay ) {
         throw txml::XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   enum_parsing_file, "Couldn't parse internal doc: '" + _filename + "'" );
      }

      collect_element_keys( &_doc );
      _isparsed = true;
   }

   namespace {
      void loadFile( std::list<string>  &l, string const& name ) {
         try {
            rlf_txtrw::t_text_read()( name, l );
         } catch( rlf_txtrw::bad_text_read& ex ) {
            string msg = string( "error reading XML file: " ) + ex.what();
            throw txml::XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                      enum_reading_file, msg );
         }
      }
   }


   void tXmlInterfaceImpl::create_root( std::string const& name, std::string const& root ) {
      root_name = root;
      std::list<std::string> text;
      text.push_back( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" );
      text.push_back( "<"  + root + ">" );
      text.push_back( "</" + root + ">" );
      bool overwrite = true;
      rlf_txtrw::t_write_ascii()( name, text, overwrite );
   }

   void tXmlInterfaceImpl::parse( string const& fn ) {
      if( fn.size() != 0 ) {
         if( fn != _filename ) {
            _filename = fn;
            _doc.clear();
            _isparsed = false;
            std::list<string>  l;
            loadFile( l, _filename );
            bool loadOkay = _doc.parseStart( l );

            if( !loadOkay ) {
               throw txml::XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                         txml::enum_parsing_file, "Couldn't parse file: '" + _filename + "'" );
            }

            collect_element_keys( &_doc );
            _isparsed = true;
         }
      }
   }

   string tXmlInterfaceImpl::getAt( string const& key )const {
      if( !_isparsed ) {
         string msg = "no parsed document found";
         throw txml::XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   txml::enum_no_parsed_document_found, msg );
      }

      txml::XmlLocator p( key );
      _doc.accept( &p );
      string value = p.value();
      string attr = p.attr();

      if( !attr.empty() ) {
         return attr;
      }

      return value;
   }

   string tXmlInterfaceImpl::addNotFoundNodeWithDefaultValue( txml::XmlNode* node,  txml::keyentries remainder, string default_ ) {


      size_t size = remainder.size();
      txml::XmlElement* nnbase = nullptr;
      txml::XmlElement* first = nullptr;

      for( size_t i = 0; i < size; i++ ) {

         txml::keyentry key_entry = remainder[i];
         string vs = key_entry.Element();
         txml::XmlElement* element = txml::XmlElement::create( vs );

         if( i == size - 1 ) {
            element->linkEndChild( txml::XmlText::create( default_ ) );
         }

         if( i == 0 ) {
            first = element;
         }

         if( nnbase != nullptr ) {
            nnbase->linkEndChild( element );
         }

         nnbase = element;

      }

      node->linkEndChild( first );

      return default_;

   }




   XmlNode* tXmlInterfaceImpl::findSmallestInsertPointForNewNode( txml::XmlLocator& p ) {

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
         std::cout << default_ << endl;

      }

      if( !_isparsed ) {
         string msg = "no parsed document found";
         throw txml::XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   txml::enum_no_parsed_document_found, msg );
      }

      txml::XmlLocator locator( key );

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

      txml::XmlNode* node = findSmallestInsertPointForNewNode( locator );

      if( node == nullptr ) {
         txml::XmlException ex( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                txml::enum_cannot_insert_root_as_default,
                                txml::msg_cannot_insert_root_as_default
                                + ", missingKey = " + key );
         throw ex;

      }

      if( key_entry.is_attr() ) {
         txml::XmlElement* elem = dynamic_cast<XmlElement*>( node );
         elem->setAttribute( key_entry.Attr(), default_ );
      } else {
         v = addNotFoundNodeWithDefaultValue( node, locator.remainder, default_ );
      }

      // rebuild internal structure
      parse();
      txml::XmlLocator locator2( key );
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
         throw txml::XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   txml::enum_no_parsed_document_found,  msg );
      }

      txml::XmlLocator p( key );

      if( p.isAttr() ) {

         throw txml::XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   txml::enum_key_points_not_to_an_element, txml::msg_key_points_not_to_an_element + ": '" + key + "'" );
      }

      _doc.accept( &p );

      if( p.accepted() ) {
         XmlElement* e = p.elementfound();
         e->setText( value );
         return;
      }

      throw txml::XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                txml::enum_key_not_found, "key not found: '" + key + "'" );
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
         throw txml::XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   txml::enum_no_parsed_document_found, msg );
      }

      txml::XmlLocator p( key );

      if( !p.isAttr() ) {
         throw txml::XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   txml::enum_key_points_not_to_an_attribute, "key didn't point to an attribute: '" + key + "'" );
      }

      _doc.accept( &p );

      if( p.accepted() ) {
         p.setElementAttr( value );
         save();
         return;
      }

      throw txml::XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                txml::enum_key_not_found, "key not found: '" + key + "'" );
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
         throw txml::XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   txml::enum_no_parsed_document_found, msg );
      }

      txml::XmlLocator p( key );

      if( p.isAttr() ) {
         throw txml::XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   txml::enum_key_points_not_to_an_element, "key didn't point to an element: '" + key + "'" );
      }

      _doc.accept( &p );

      if( p.accepted() ) {
         XmlElement* e = p.elementfound();
         txml::keyentries const& lookuppath = e->lookuppath();
         boost::scoped_ptr<txml::XmlNode> comment( txml::XmlComment::create( t_alloc_line_file_method( __LINE__, __FILE__, __FUNCTION__ ) ) );
         comment->value( value );
         txml::XmlNode const* parent   = lookuppath.parentOfLast();

         if( parent != nullptr ) {
            const_cast<txml::XmlNode*>( parent )->insertBeforeChild( e, *comment );
         }

         return;
      }

      throw txml::XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                txml::enum_key_not_found, "key not found: '" + key + "'" );
   }

   void tXmlInterfaceImpl::save( string const& fn ) {
      string f = fn;

      if( f.empty() ) {
         f = _filename;
      }

      _doc.saveFile( f );
   }

   void tXmlInterfaceImpl::save() {
      _doc.saveFile( _filename );
   }



   void tXmlInterfaceImpl::collect_element_keys( txml::XmlNode const* node ) const {
      if( node == nullptr ) {
         return;
      }

      txml::XmlElement const* element = nullptr;

      try {
         element = dynamic_cast<XmlElement const*>( node );
      } catch( XmlException& ex ) {
         element = nullptr;
      }

      if( element != nullptr ) {
         string lp = element->lookuppath().to_string();
         collected_keys.push_back( element->lookuppath() );
         collect_attribute_keys( element );
      }

      txml::XmlNode const* child;

      for( child = node->firstChild(); child != nullptr; child = child->next() ) {
         collect_element_keys( child );
      }
   }

   void tXmlInterfaceImpl::collect_attribute_keys( txml::XmlNode const* node )const {
      txml::XmlElement const* element = dynamic_cast<XmlElement const*>( node );

      if( element == nullptr ) {
         return ;
      }

      vector<txml::XmlAttribute> const& v = element->Attributes();
      vector<txml::XmlAttribute>::const_iterator begin = v.begin();
      vector<txml::XmlAttribute>::const_iterator end = v.end();

      while( begin != end ) {
         collected_keys.push_back( begin->keys );
         ++begin;
      }
   }



   void tXmlInterfaceImpl::printKeys( string const& fn )const {
      std::vector<std::string> v = Keys();
      std::list<std::string> l( v.begin(), v.end() );
      bool overwrite = true;
      rlf_txtrw::t_write_ascii()( fn, l, overwrite );
   }


   std::vector<std::string> tXmlInterfaceImpl::Keys()const {
      std::vector<std::string> v;
      std::vector<txml::keyentries>::const_iterator begin = collected_keys.begin();
      std::vector<txml::keyentries>::const_iterator end = collected_keys.end();

      while( begin != end ) {
         string temp = begin->to_string();
         v.push_back( temp );
         ++begin;
      }

      return v;
   }



   void tXmlInterfaceImpl::printKeysAndValues( string const& fn, size_t w )const {
      std::list<std::string> l;
      std::vector<txml::keyentries>::const_iterator begin = collected_keys.begin();
      std::vector<txml::keyentries>::const_iterator end = collected_keys.end();

      while( begin != end ) {
         string temp = begin->to_string();
         txml::keyentry const& pe = begin->last();
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
         ++begin;
      }

      bool overwrite = true;
      rlf_txtrw::t_write_ascii()( fn, l, true );
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
      string cleanup_string_const( string const& n ) {
         string line = n;
         line = rlf_hstring::replace_all( line, ".", "_" );
         line = rlf_hstring::replace_all( line, "-", "_" );
         line = rlf_hstring::replace_all( line, ":", "__" );
         line = rlf_hstring::replace_all( line, "[", "__" );
         line = rlf_hstring::replace_all( line, "]", "" );
         return line;

      }
   }



   void tXmlInterfaceImpl::printXmlGenerator( string const& fn_keys, std::string const& fn_out )const {
      std::list<std::string> l;
      std::vector<txml::keyentries>::const_iterator begin = collected_keys.begin();
      std::vector<txml::keyentries>::const_iterator end = collected_keys.end();
      string root;
      vector<string> root_attr;


      while( begin != end ) {
         //txml::keyentries const& ke = *begin;
         txml::keyentry const& pe = begin->last();
         string val = pe.toValue();
         string line = begin->to_string();
         string lineValue = line;
         ++begin;

         if( root.size() == 0 ) {
            root = lineValue;
            continue;
         }

         if( line.find( root + ":" ) != string::npos ) {
            string attr = lineValue;
            attr = rlf_hstring::replace_all( attr, root + ":", "" ) ;
            attr += "=";
            attr += "\"" + val + "\"";
            root_attr.push_back( attr );
            continue;
         }

         line = cleanup_string_const( line );

         string temp = "const string ";
         temp += line + " = \"";

         temp += lineValue + "\";";

         string txt = "  element:   ";

         if( pe.is_attr() ) {
            txt =     "  attribute: ";
         }

         string space = "  // ";
         size_t s = 1; // insert only one '-'

         while( ( space.size() + txt.size() + temp.size() ) < s ) {
            space += "-";
         }

         string final_line = temp + space + txt + " = '" + val + "'";
         l.push_back( final_line );


      }

      l.push_back( "" );
      l.push_back( "" );

      string attr2 = merge( root_attr, " " );

      std::list<std::string> text;
      text.push_back( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" );
      text.push_back( "<" + root + " " + attr2 + "/>" );

      bool overwrite = true;
      rlf_txtrw::t_write_ascii()( fn_out, text, overwrite );


      l.push_back( "xmlinterface::tXmlInterface instance;" );
      l.push_back( "instance.parse( xmlTestOut );" );



      begin = collected_keys.begin();
      end = collected_keys.end();

      root = string();

      while( begin != end ) {
         txml::keyentry const& pe = begin->last();
         string val = pe.toValue();

         string line = begin->to_string();
         string lineValue = line;
         ++begin;

         if( root.size() == 0 ) {
            root = lineValue;
            continue;
         }

         if( line.find( root + ":" ) != string::npos ) {
            continue;
         }


         line = cleanup_string_const( line );

         string final_line = "instance.create( ";
         final_line += line + ", \"" + val + "\" );";
         l.push_back( final_line );


      }

      l.push_back( "instance.save();" );
      rlf_txtrw::t_write_ascii()( fn_keys, l, overwrite );
   }

   void tXmlInterfaceImpl::create( string const& key, string const& value ) {

      get_string( key, value );

   }



   void showAllocList() {
      alloccheck::tAllocMap const& allocList = alloccheck::alloccheckInstance().getAlloclist();

      if( allocList.size() > 0 ) {
         alloccheck::allocIterator itstart = allocList.begin();
         alloccheck::allocIterator itend = allocList.end();

         while( itstart != itend ) {
            pair<size_t, string> pa = *itstart;
            string info = pa.second;
            ++itstart;
         }
      }

      LOGT_A_INFO( " end of program, number of not deleted pointers: " + rlf_hstring::toString( ( int )allocList.size() ) );

   }



   int str_to_int( std::string const& s )  {
      return xmlinterface::to_int( s ) ;
   }
   double str_to_double( std::string const& s )  {
      return xmlinterface::to_double( s );
   }

   std::string int_to_string( int val )  {
      return rlf_hstring::toString( val );
   }


   std::string double_to_string( double val )  {
      return rlf_hstring::trim( rlf_hstring::toString( val ) );
   }


   std::vector<std::string> split( std::string const& l, std::string const& pat ) {
      return rlf_hstring::split( l, pat );
   }
   std::string replace_all( std::string const& ins, const std::string& pattern, const std::string& replace ) {
      return rlf_hstring::replace_all( ins, pattern, replace );
   }
   size_t index( std::string const& s, std::string const& pattern, size_t pos ) {
      return rlf_hstring::index( s, pattern, pos );
   }
   std::string trim( std::string const& str, char ch ) {
      return rlf_hstring::trim( str, ch );
   }


   std::string fillup( std::string const& in, char ch, size_t n ) {
      return rlf_hstring::fillup( in, ch, n );
   }
   std::string to_string( double val ) {
      return rlf_hstring::toString( val );
   }
   std::string to_string( int val ) {
      return rlf_hstring::toString( val );
   }
//   int to_int( std::string const& s ) {
//      return rlf_hstring::to_int( s );
//   }
//   // double to_double( std::string const& s, std::locale l ) {
//   // return strings::to_double( s, l );
//   //}
//   double to_double( std::string const& s ) {
//      return rlf_hstring::to_double( s );
//   }

}


// forward to tXmlInterfaceImpl

namespace xmlinterface {

   int to_int( std::string const& s )  {
      try {
         return boost::lexical_cast<int>( s );
      } catch( boost::bad_lexical_cast& e ) {
         string msg = e.what();
         throw txml::XmlException( txml::t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   txml::enum_bad_lexical_cast, "msg: " + msg );
      }

   }

   double to_double( std::string const& s )  {
      try {
         double b = boost::lexical_cast<double>( rlf_hstring::trim( s ) );
         return b;
      } catch( boost::bad_lexical_cast& e ) {
         string msg = e.what();
         throw txml::XmlException( txml::t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   txml::enum_bad_lexical_cast, "string : " + s + ", msg: " + msg );
      }
   }



   tXmlInterface::tXmlInterface(): impl( new txml::tXmlInterfaceImpl() ) {}


   tXmlInterface::~tXmlInterface() {
      delete impl;
   }

   void tXmlInterface::parse( txml::XmlDocument const& doc ) {
      impl->parse( doc );
   }


   void tXmlInterface::parse( string const& fn ) {
      impl->parse( fn );
   }


   void tXmlInterface::create_root( std::string const& name, std::string const& root ) {
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


   std::vector<std::string> tXmlInterface::Keys()const {
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


   void show_alloc_list() {
      txml::showAllocList();
   }


   int str_to_int( std::string const& s )  {
      return txml::str_to_int( s ) ;
   }
   double str_to_double( std::string const& s )  {
      return txml::str_to_double( s );
   }

   std::string int_to_string( int val )  {
      return txml::int_to_string( val );
   }

   std::string double_to_string( double val )  {
      return txml::double_to_string( val );
   }

   std::vector<std::string> split( std::string const& l, std::string const& pat ) {
      return txml::split( l, pat );
   }
   std::string replace_all( std::string const& ins, const std::string& pattern, const std::string& replace ) {
      return txml::replace_all( ins, pattern, replace );
   }
   size_t index( std::string const& s, std::string const& pattern, size_t pos ) {
      return txml::index( s, pattern, pos );
   }
   std::string trim( std::string const& str, char ch ) {
      return rlf_hstring::trim( str, ch );
   }


   std::string fillup( std::string const& in, char ch, size_t n ) {
      return rlf_hstring::fillup( in, ch, n );
   }
   std::string to_string( double val ) {
      return rlf_hstring::toString( val );
   }
   std::string to_string( int val ) {
      return rlf_hstring::toString( val );
   }
//   int to_int( std::string const& s ) {
//      return rlf_hstring::to_int( s );
//   }
//   // double to_double( std::string const& s, std::locale l ) {
//   // return strings::to_double( s, l );
//   //}
//   double to_double( std::string const& s ) {
//      return rlf_hstring::to_double( s );
//   }

}






//EOF
