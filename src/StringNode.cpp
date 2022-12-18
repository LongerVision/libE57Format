/*
 * StringNode.cpp - implementation of public functions of the StringNode class.
 *
 * Original work Copyright 2009 - 2010 Kevin Ackley (kackley@gwi.net)
 * Modified work Copyright 2018 - 2020 Andy Maloney <asmaloney@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/// @file StringNode.cpp

#include "StringFunctions.h"
#include "StringNodeImpl.h"

using namespace e57;

// Put this function first so we can reference the code in doxygen using @skip
/*!
@brief Check whether StringNode class invariant is true
@copydetails IntegerNode::checkInvariant()
*/
void StringNode::checkInvariant( bool /*doRecurse*/, bool doUpcast )
{
   // If destImageFile not open, can't test invariant (almost every call would
   // throw)
   if ( !destImageFile().isOpen() )
   {
      return;
   }

   // If requested, call Node::checkInvariant
   if ( doUpcast )
   {
      static_cast<Node>( *this ).checkInvariant( false, false );
   }
   // ? check legal UTF-8
}

/*!
@class e57::StringNode

@brief An E57 element encoding a Unicode character string value.

@details
A StringNode is a terminal node (i.e. having no children) that holds an Unicode character string
encoded in UTF-8. Once the StringNode value is set at creation, it may not be modified.

See Node class discussion for discussion of the common functions that StructureNode supports.

@section StringNode_invariant Class Invariant
A class invariant is a list of statements about an object that are always true before and after any
operation on the object. An invariant is useful for testing correct operation of an implementation.
Statements in an invariant can involve only externally visible state, or can refer to internal
implementation-specific state that is not visible to the API user. The following C++ code checks
externally visible state for consistency and throws an exception if the invariant is violated:

@dontinclude StringNode.cpp
@skip void StringNode::checkInvariant
@until ^}

@see Node
*/

/*!
@brief Create an element storing a Unicode character string.

@param [in] destImageFile The ImageFile where the new node will eventually be stored.
@param [in] value The Unicode character string value of the element, in UTF-8 encoding.

@details
The StringNode class corresponds to the ASTM E57 standard String element. See the class discussion
at bottom of StringNode page for more details.

The @a destImageFile indicates which ImageFile the StringNode will eventually be attached to. A node
is attached to an ImageFile by adding it underneath the predefined root of the ImageFile (gotten
from ImageFile::root). It is not an error to fail to attach the StringNode to the @a destImageFile.
It is an error to attempt to attach the StringNode to a different ImageFile.

If the StringNode is to be used in a CompressedVectorNode prototype, it is recommended to specify a
@a value = "" (the default value).

@pre The @a destImageFile must be open (i.e. destImageFile.isOpen() must be true).
@pre The @a destImageFile must have been opened in write mode (i.e. destImageFile.isWritable() must
be true).

@throw ::ErrorImageFileNotOpen
@throw ::ErrorFileReadOnly
@throw ::ErrorInternal All objects in undocumented state

@see StringNode::value, Node, CompressedVectorNode, CompressedVectorNode::prototype
*/
StringNode::StringNode( const ImageFile &destImageFile, const ustring &value ) :
   impl_( new StringNodeImpl( destImageFile.impl(), value ) )
{
}

/*!
@brief Is this a root node.
@copydetails Node::isRoot()
*/
bool StringNode::isRoot() const
{
   return impl_->isRoot();
}

/*!
@brief Return parent of node, or self if a root node.
@copydetails Node::parent()
*/
Node StringNode::parent() const
{
   return Node( impl_->parent() );
}

/*!
@brief Get absolute pathname of node.
@copydetails Node::pathName()
*/
ustring StringNode::pathName() const
{
   return impl_->pathName();
}

/*!
@brief Get elementName string, that identifies the node in its parent.
@copydetails Node::elementName()
*/
ustring StringNode::elementName() const
{
   return impl_->elementName();
}

/*!
@brief Get the ImageFile that was declared as the destination for the node when it was created.
@copydetails Node::destImageFile()
*/
ImageFile StringNode::destImageFile() const
{
   return ImageFile( impl_->destImageFile() );
}

/*!
@brief Has node been attached into the tree of an ImageFile.
@copydetails Node::isAttached()
*/
bool StringNode::isAttached() const
{
   return impl_->isAttached();
}

/*!
@brief Get Unicode character string value stored.

@pre The destination ImageFile must be open (i.e. destImageFile().isOpen()).

@post No visible state is modified.

@return  The Unicode character string value stored.

@throw ::ErrorImageFileNotOpen
@throw ::ErrorInternal All objects in undocumented state
*/
ustring StringNode::value() const
{
   return impl_->value();
}

/*!
@brief Diagnostic function to print internal state of object to output stream in an indented format.
@copydetails Node::dump()
*/
#ifdef E57_DEBUG
void StringNode::dump( int indent, std::ostream &os ) const
{
   impl_->dump( indent, os );
}
#else
void StringNode::dump( int indent, std::ostream &os ) const
{
}
#endif

/*!
@brief Upcast a StringNode handle to a generic Node handle.

@details
An upcast is always safe, and the compiler can automatically insert it for initializations of Node
variables and Node function arguments.

@return  A smart Node handle referencing the underlying object.

@throw No E57Exceptions.

@see Explanation in Node, Node::type(), StringNode(const Node&)
*/
StringNode::operator Node() const
{
   /// Upcast from shared_ptr<StringNodeImpl> to SharedNodeImplPtr and construct
   /// a Node object
   return Node( impl_ );
}

/*!
@brief Downcast a generic Node handle to a StringNode handle.

@param [in] n The generic handle to downcast.

@details
The handle @a n must be for an underlying StringNode, otherwise an exception is thrown. In designs
that need to avoid the exception, use Node::type() to determine the actual type of the @a n before
downcasting. This function must be explicitly called (c++ compiler cannot insert it automatically).

@throw ::ErrorBadNodeDowncast

@see Node::type(), StringNode::operator Node()
*/
StringNode::StringNode( const Node &n )
{
   if ( n.type() != TypeString )
   {
      throw E57_EXCEPTION2( ErrorBadNodeDowncast, "nodeType=" + toString( n.type() ) );
   }

   /// Set our shared_ptr to the downcast shared_ptr
   impl_ = std::static_pointer_cast<StringNodeImpl>( n.impl() );
}

/// @cond documentNonPublic The following isn't part of the API, and isn't documented.
StringNode::StringNode( std::shared_ptr<StringNodeImpl> ni ) : impl_( ni )
{
}
/// @endcond
