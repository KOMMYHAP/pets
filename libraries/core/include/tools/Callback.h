#pragma once

#include <utility>		// forward
#include <tuple>		// tuple, apply
#include <any>			// any, any_cast, make_any
#include <functional>	// function
#include <iostream>		// cerr
#include <memory>		// unique_ptr, weak_ptr

#include "TemplateUtils.h" 	// always_false_v

class CallbackBase
{
public:
	virtual ~CallbackBase() = default;

	/**
	 * \brief Checks types of passed args and passed them to the function if types are the same and owner is still alive.
	 * \param args Arguments packed in tuple.
	 * \note This function should not be called manually, it's used for internal purposes.
	 */
	virtual void Do(std::any args) const = 0;
	
	/**
	 * \return true if callback is callable and not expired.
	 */
	virtual bool Valid() const = 0;
};

template <class ...Args>
class TypedCallback : public CallbackBase
{
public:
	TypedCallback() = default;

	template<class Base, class Derived>
	TypedCallback(std::weak_ptr<void> owner, Base * base, void (Derived::*f)(Args...)) noexcept
		: _owner(std::move(owner))
	{
		_function = [base, f](const Args &... args)
		{
			std::invoke(f, base, args...);
		};
	}

	template<class Base, class Derived>
	TypedCallback(std::weak_ptr<Base> owner, void (Derived::*f)(Args...)) noexcept
		: _owner(std::move(owner))
	{
		_function = [f, this](const Args &... args)
		{
			std::invoke(f, _owner.lock(), args...);
		};
	}
	
	TypedCallback(std::weak_ptr<void> owner, std::function<void(Args...)> f) noexcept
		: _owner(std::move(owner))
		, _function(std::move(f))
	{}

	/**
	 * \brief Directly passes args to the function if owner is still alive.
	 * \param args Arguments to be passed to the function.
	 */
	void operator()(const Args &... args) const
	{
		if (Valid())
		{
			std::invoke(_function, args...);
		}
	}

	/**
	 * \brief Checks types of passed args and passed them to the function if types are the same and owner is still alive.
	 * \param args Arguments packed in tuple.
	 * \note This function should not be called manually, it's used for internal purposes.
	 */
	void Do(std::any args) const override
	{
		if (Valid())
		{
			bool canCasted = std::any_cast<std::tuple<Args...>>(&args) != nullptr;
			if (canCasted)
			{
				std::apply(_function, std::any_cast<std::tuple<Args...>>(std::move(args)));
			}
			else
			{
				std::cerr << typeid(*this).name() << ": Types mismatched, args are " << args.type().name() << ".\n";
			}
		}
	}

	bool Valid() const override
	{
		return !_owner.expired() && _function;
	}

	auto Owner() const { return _owner; }

private:
	std::weak_ptr<void> _owner;
	std::function<void(Args...)> _function;
};

class Callback
{
public:
	Callback() = default;

	template<class ...Args>
	Callback(std::weak_ptr<void> owner, std::function<void(Args...)> f)
		: _delegate(std::make_unique<TypedCallback<Args...>>(
			  std::move(owner)
			, std::move(f)))
	{}

	Callback(std::weak_ptr<void> owner, std::function<void()> f)
		: _delegate(std::make_unique<TypedCallback<>>(
			  std::move(owner)
			, std::move(f)))
	{}


	/**
	 * \brief Downcasts typed callback.
	 * \param typedCallback Typed callback to be moved.
	 */
	template<class ...Args>
	Callback(TypedCallback<Args...> typedCallback)
		: _delegate(std::make_unique<TypedCallback<Args...>>(std::move(typedCallback)))
	{}

	/**
	 * \brief Checks types of passed args and types of stored function and apply them to the function if types are the same and owner is still alive.
	 * \param args Arguments to be packed in tuple and to be applied to the function.
	 * \note Move-only types are unsupported and should be avoided.
	 */
	template<class ...Args>
	void operator()(Args &&... args) const
	{
		if constexpr((... && std::is_copy_constructible_v<Args>))
		{
			_delegate->Do(std::make_any<std::tuple<Args...>>(std::forward<Args>(args)...));
		}
		else
		{
			static_assert(Utils::always_false_v<Args...>, "Move-only types are unsupported in std::any.");
		}
	}
	
	bool Valid() const
	{
		return _delegate && _delegate->Valid();
	}

private:
	std::unique_ptr<CallbackBase> 	_delegate;
};
