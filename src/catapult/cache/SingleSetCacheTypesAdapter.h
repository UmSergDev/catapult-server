/**
*** Copyright (c) 2016-present,
*** Jaguar0625, gimre, BloodyRookie, Tech Bureau, Corp. All rights reserved.
***
*** This file is part of Catapult.
***
*** Catapult is free software: you can redistribute it and/or modify
*** it under the terms of the GNU Lesser General Public License as published by
*** the Free Software Foundation, either version 3 of the License, or
*** (at your option) any later version.
***
*** Catapult is distributed in the hope that it will be useful,
*** but WITHOUT ANY WARRANTY; without even the implied warranty of
*** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*** GNU Lesser General Public License for more details.
***
*** You should have received a copy of the GNU Lesser General Public License
*** along with Catapult. If not, see <http://www.gnu.org/licenses/>.
**/

#pragma once
#include "CacheDatabaseMixin.h"
#include "catapult/deltaset/ConditionalContainer.h"
#include <type_traits>

namespace catapult { namespace cache {

	/// A cache types adapter for a cache composed of a single set.
	template<typename TPrimaryTypes, typename IsOrderedFlag = std::false_type>
	struct SingleSetCacheTypesAdapter : public CacheDatabaseMixin {
	public:
		using PrimaryTypes = TPrimaryTypes;

	public:
		/// Wrapper around single delta set.
		struct BaseSetDeltaPointers {
			typename TPrimaryTypes::BaseSetDeltaPointerType pPrimary;
		};

		/// Wrapper around single set.
		struct BaseSets : public CacheDatabaseMixin {
		public:
			/// Indicates the set is ordered (used for capability detection in templates).
			using IsOrderedSet = IsOrderedFlag;

		public:
			/// Creates base sets around \a config.
			explicit BaseSets(const CacheConfiguration& config)
					: CacheDatabaseMixin(config, { "default" })
					, Primary(GetContainerMode(config), database(), 0)
			{}

		public:
			typename TPrimaryTypes::BaseSetType Primary;

		public:
			/// Returns a delta based on the same original elements as this set.
			BaseSetDeltaPointers rebase() {
				return { Primary.rebase() };
			}

			/// Returns a delta based on the same original elements as this set
			/// but without the ability to commit any changes to the original set.
			BaseSetDeltaPointers rebaseDetached() const {
				return { Primary.rebaseDetached() };
			}

			/// Commits all changes in the rebased cache.
			/// \a args are forwarded to the commit policy.
			template<typename... TArgs>
			void commit(TArgs&&... args) {
				Primary.commit(std::forward<TArgs>(args)...);
			}
		};
	};
}}
