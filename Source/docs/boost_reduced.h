namespace boost
{
	//! Boost class.
	template< class _Type > struct shared_ptr{ _Type element; };
	//! Boost class.
	template< class _Type > struct scoped_ptr{ _Type element; };
	//! Boost class.
	template< class _Type > struct weak_ptr{ _Type element; };

	//! Boost class.
	template< class _Type > struct shared_array{ _Type element; };
	//! Boost class.
	template< class _Type > struct scoped_array{ _Type element; };

	//! Boost class.
	template< class _Type > struct array{ _Type element; };
};