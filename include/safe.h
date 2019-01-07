/*
 * safe.h
 *
 *  Created on: Sep 21, 2018
 *      Author: lcc
 */

#ifndef SAFE_H_
#define SAFE_H_

#include <mutex>
#include <type_traits>

namespace safe
{
	template<typename, typename> class Access;
	template<typename, typename> class Safe;
}  // namespace safe

namespace safe
{
	struct default_construct_lockable {};
	
	/**
	 * @brief A class that wraps a value and a mutex
	 * object to protect the value.
	 * 
	 * The value is hidden by the Safe class, although it can be
	 * accessed using the unsafe() function. The safe way to access
	 * the value is through the Lock and Guard classes.
	 * 
	 * @tparam ValueType The type of the shared value to protects.
	 * @tparam LockableType The type of the lockable object.
	 */
	template<typename ValueType, typename LockableType = std::mutex>
	class Safe
	{
		using RemoveRefValueType = typename std::remove_reference<ValueType>::type;
		using RemoveRefLockableType = typename std::remove_reference<LockableType>::type;
		
		/**
		 * @brief A class that locks a Safe object with unique lock behavior and
		 * gives access to the Safe object's value.
		 */
		template<typename LockType, bool Shared>
		class AccessImpl
		{
		private:
			using AccessValueType = typename std::conditional<Shared, const RemoveRefValueType, RemoveRefValueType>::type;

		public:
			using ConstPointerType = const AccessValueType*;
			using PointerType = AccessValueType*;
			using ConstReferenceType = const AccessValueType&;
			using ReferenceType = AccessValueType&;

			/**
			 * @brief Locks safe with unique lock behavior and gives
			 * access to its value.
			 * 
			 * @param[in] safe The Safe object to lock.
			 */
			AccessImpl(const Safe& safe) noexcept;
			/**
			 * @brief Locks safe with unique lock behavior and gives
			 * access to its value.
			 * 
			 * @param[in] safe The Safe object to lock.
			 */
			AccessImpl(Safe& safe) noexcept;

			/**
			 * @brief Const accessor function.
			 * @return ConstValuePointerType The protected value.
			 */
			ConstPointerType operator->() const noexcept;

			/**
			 * @brief Accessor function.
			 * @return ValuePointerType The protected value.
			 */
			PointerType operator->() noexcept;

			/**
			 * @brief Const accessor function.
			 * @return ConstValueReferenceType The protected value.
			 */
			ConstReferenceType operator*() const noexcept;

			/**
			 * @brief Const accessor function.
			 * @return ValueReferenceType The protected value.
			 */
			ReferenceType operator*() noexcept;

			const LockType& getLock() const noexcept;
			LockType& getLock() noexcept;

		private:
			/// The lock that manages the lockable object.
			LockType m_lock;
			/// The protected value.
			ReferenceType m_value;
		};

	public:
		using ConstReferenceType = const RemoveRefValueType&;
		using ReferenceType = RemoveRefValueType&;

		template<template<typename> class LockType>
		using SharedAccess = AccessImpl<LockType<RemoveRefLockableType>, true>;
		template<template<typename> class LockType>
		using Access = AccessImpl<LockType<RemoveRefLockableType>, false>;
		
		/**
		 * @brief Construct a new Safe object
		 */
		Safe() = default;
		/**
		 * @brief Construct a new Safe object with default construction of the lockable
		 * object and perfect forwarding of the arguments of the value's constructor.
		 * 
		 * @tparam ValueArgs Perfect forwarding types to construct the value.
		 * @param tag Indicates that the lockable object should be default constructed.
		 * @param valueArgs Perfect forwarding arguments to construct the value.
		 */
		template<typename... ValueArgs>
		Safe(default_construct_lockable tag, ValueArgs&&... valueArgs);
		/**
		 * @brief Construct a new Safe object, perfect forwarding one argument to the
		 * lockable's constructor and perfect forwarding the other arguments
		 * of the value's constructor.
		 * 
		 * @tparam LockableArg Perfect forwarding type to construct the lockable.
		 * @tparam ValueArgs Perfect forwarding types to construct the value.
		 * @param lockableArg Perfect forwarding argument to construct the lockable.
		 * @param valueArgs Perfect forwarding arguments to construct the value.
		 */
		template<typename LockableArg, typename... ValueArgs>
		Safe(LockableArg&& lockableArg, ValueArgs&&... valueArgs);

		/**
		 * @brief %Safe access to the protected value through a SharedGuard object.
		 * 
		 * @return SharedGuard
		 */
		template<template<typename> class LockType>
		SharedAccess<LockType> accessShared() const noexcept;
		template<template<typename> class LockType>
		SharedAccess<LockType> access() const noexcept;
		template<template<typename> class LockType>
		Access<LockType> access() noexcept;
		/**
		 * @brief Unsafe const accessor function.
		 * 
		 * @return ConstValueReferenceType The unprotected value.
		 */
		ConstReferenceType unsafe() const noexcept;
		/**
		 * @brief Unsafe accessor function.
		 * 
		 * @return ValueReferenceType The unprotected value.
		 */
		ReferenceType unsafe() noexcept;

	private:
		/// The lockable object that protects the value.
		LockableType m_lockable;
		/// The value to protect.
		ValueType m_value;
	};
}  // namespace safe

#endif /* SAFE_H_ */
