﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/greengrass/Greengrass_EXPORTS.h>
#include <aws/greengrass/GreengrassRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Greengrass
{
namespace Model
{

  /**
   */
  class AWS_GREENGRASS_API DeleteCoreDefinitionRequest : public GreengrassRequest
  {
  public:
    DeleteCoreDefinitionRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DeleteCoreDefinitionRequest> Clone() const
    {
      return Aws::MakeUnique<DeleteCoreDefinitionRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DeleteCoreDefinition"; }

    Aws::String SerializePayload() const override;


    /**
     * The ID of the core definition.
     */
    inline const Aws::String& GetCoreDefinitionId() const{ return m_coreDefinitionId; }

    /**
     * The ID of the core definition.
     */
    inline bool CoreDefinitionIdHasBeenSet() const { return m_coreDefinitionIdHasBeenSet; }

    /**
     * The ID of the core definition.
     */
    inline void SetCoreDefinitionId(const Aws::String& value) { m_coreDefinitionIdHasBeenSet = true; m_coreDefinitionId = value; }

    /**
     * The ID of the core definition.
     */
    inline void SetCoreDefinitionId(Aws::String&& value) { m_coreDefinitionIdHasBeenSet = true; m_coreDefinitionId = std::move(value); }

    /**
     * The ID of the core definition.
     */
    inline void SetCoreDefinitionId(const char* value) { m_coreDefinitionIdHasBeenSet = true; m_coreDefinitionId.assign(value); }

    /**
     * The ID of the core definition.
     */
    inline DeleteCoreDefinitionRequest& WithCoreDefinitionId(const Aws::String& value) { SetCoreDefinitionId(value); return *this;}

    /**
     * The ID of the core definition.
     */
    inline DeleteCoreDefinitionRequest& WithCoreDefinitionId(Aws::String&& value) { SetCoreDefinitionId(std::move(value)); return *this;}

    /**
     * The ID of the core definition.
     */
    inline DeleteCoreDefinitionRequest& WithCoreDefinitionId(const char* value) { SetCoreDefinitionId(value); return *this;}

  private:

    Aws::String m_coreDefinitionId;
    bool m_coreDefinitionIdHasBeenSet = false;
  };

} // namespace Model
} // namespace Greengrass
} // namespace Aws
