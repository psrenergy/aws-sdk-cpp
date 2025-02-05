﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/iottwinmaker/IoTTwinMaker_EXPORTS.h>
#include <aws/iottwinmaker/model/GroupType.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Utils
{
namespace Json
{
  class JsonValue;
  class JsonView;
} // namespace Json
} // namespace Utils
namespace IoTTwinMaker
{
namespace Model
{

  /**
   * <p>The component property group response.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/iottwinmaker-2021-11-29/ComponentPropertyGroupResponse">AWS
   * API Reference</a></p>
   */
  class AWS_IOTTWINMAKER_API ComponentPropertyGroupResponse
  {
  public:
    ComponentPropertyGroupResponse();
    ComponentPropertyGroupResponse(Aws::Utils::Json::JsonView jsonValue);
    ComponentPropertyGroupResponse& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p>The group type.</p>
     */
    inline const GroupType& GetGroupType() const{ return m_groupType; }

    /**
     * <p>The group type.</p>
     */
    inline bool GroupTypeHasBeenSet() const { return m_groupTypeHasBeenSet; }

    /**
     * <p>The group type.</p>
     */
    inline void SetGroupType(const GroupType& value) { m_groupTypeHasBeenSet = true; m_groupType = value; }

    /**
     * <p>The group type.</p>
     */
    inline void SetGroupType(GroupType&& value) { m_groupTypeHasBeenSet = true; m_groupType = std::move(value); }

    /**
     * <p>The group type.</p>
     */
    inline ComponentPropertyGroupResponse& WithGroupType(const GroupType& value) { SetGroupType(value); return *this;}

    /**
     * <p>The group type.</p>
     */
    inline ComponentPropertyGroupResponse& WithGroupType(GroupType&& value) { SetGroupType(std::move(value)); return *this;}


    /**
     * <p>The names of properties</p>
     */
    inline const Aws::Vector<Aws::String>& GetPropertyNames() const{ return m_propertyNames; }

    /**
     * <p>The names of properties</p>
     */
    inline bool PropertyNamesHasBeenSet() const { return m_propertyNamesHasBeenSet; }

    /**
     * <p>The names of properties</p>
     */
    inline void SetPropertyNames(const Aws::Vector<Aws::String>& value) { m_propertyNamesHasBeenSet = true; m_propertyNames = value; }

    /**
     * <p>The names of properties</p>
     */
    inline void SetPropertyNames(Aws::Vector<Aws::String>&& value) { m_propertyNamesHasBeenSet = true; m_propertyNames = std::move(value); }

    /**
     * <p>The names of properties</p>
     */
    inline ComponentPropertyGroupResponse& WithPropertyNames(const Aws::Vector<Aws::String>& value) { SetPropertyNames(value); return *this;}

    /**
     * <p>The names of properties</p>
     */
    inline ComponentPropertyGroupResponse& WithPropertyNames(Aws::Vector<Aws::String>&& value) { SetPropertyNames(std::move(value)); return *this;}

    /**
     * <p>The names of properties</p>
     */
    inline ComponentPropertyGroupResponse& AddPropertyNames(const Aws::String& value) { m_propertyNamesHasBeenSet = true; m_propertyNames.push_back(value); return *this; }

    /**
     * <p>The names of properties</p>
     */
    inline ComponentPropertyGroupResponse& AddPropertyNames(Aws::String&& value) { m_propertyNamesHasBeenSet = true; m_propertyNames.push_back(std::move(value)); return *this; }

    /**
     * <p>The names of properties</p>
     */
    inline ComponentPropertyGroupResponse& AddPropertyNames(const char* value) { m_propertyNamesHasBeenSet = true; m_propertyNames.push_back(value); return *this; }


    /**
     * <p>A Boolean value that specifies whether the property group is inherited from a
     * parent entity</p>
     */
    inline bool GetIsInherited() const{ return m_isInherited; }

    /**
     * <p>A Boolean value that specifies whether the property group is inherited from a
     * parent entity</p>
     */
    inline bool IsInheritedHasBeenSet() const { return m_isInheritedHasBeenSet; }

    /**
     * <p>A Boolean value that specifies whether the property group is inherited from a
     * parent entity</p>
     */
    inline void SetIsInherited(bool value) { m_isInheritedHasBeenSet = true; m_isInherited = value; }

    /**
     * <p>A Boolean value that specifies whether the property group is inherited from a
     * parent entity</p>
     */
    inline ComponentPropertyGroupResponse& WithIsInherited(bool value) { SetIsInherited(value); return *this;}

  private:

    GroupType m_groupType;
    bool m_groupTypeHasBeenSet = false;

    Aws::Vector<Aws::String> m_propertyNames;
    bool m_propertyNamesHasBeenSet = false;

    bool m_isInherited;
    bool m_isInheritedHasBeenSet = false;
  };

} // namespace Model
} // namespace IoTTwinMaker
} // namespace Aws
