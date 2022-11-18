﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/lightsail/Lightsail_EXPORTS.h>
#include <aws/lightsail/LightsailRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Lightsail
{
namespace Model
{

  /**
   */
  class AWS_LIGHTSAIL_API GetRelationalDatabaseRequest : public LightsailRequest
  {
  public:
    GetRelationalDatabaseRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<GetRelationalDatabaseRequest> Clone() const
    {
      return Aws::MakeUnique<GetRelationalDatabaseRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetRelationalDatabase"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The name of the database that you are looking up.</p>
     */
    inline const Aws::String& GetRelationalDatabaseName() const{ return m_relationalDatabaseName; }

    /**
     * <p>The name of the database that you are looking up.</p>
     */
    inline bool RelationalDatabaseNameHasBeenSet() const { return m_relationalDatabaseNameHasBeenSet; }

    /**
     * <p>The name of the database that you are looking up.</p>
     */
    inline void SetRelationalDatabaseName(const Aws::String& value) { m_relationalDatabaseNameHasBeenSet = true; m_relationalDatabaseName = value; }

    /**
     * <p>The name of the database that you are looking up.</p>
     */
    inline void SetRelationalDatabaseName(Aws::String&& value) { m_relationalDatabaseNameHasBeenSet = true; m_relationalDatabaseName = std::move(value); }

    /**
     * <p>The name of the database that you are looking up.</p>
     */
    inline void SetRelationalDatabaseName(const char* value) { m_relationalDatabaseNameHasBeenSet = true; m_relationalDatabaseName.assign(value); }

    /**
     * <p>The name of the database that you are looking up.</p>
     */
    inline GetRelationalDatabaseRequest& WithRelationalDatabaseName(const Aws::String& value) { SetRelationalDatabaseName(value); return *this;}

    /**
     * <p>The name of the database that you are looking up.</p>
     */
    inline GetRelationalDatabaseRequest& WithRelationalDatabaseName(Aws::String&& value) { SetRelationalDatabaseName(std::move(value)); return *this;}

    /**
     * <p>The name of the database that you are looking up.</p>
     */
    inline GetRelationalDatabaseRequest& WithRelationalDatabaseName(const char* value) { SetRelationalDatabaseName(value); return *this;}

  private:

    Aws::String m_relationalDatabaseName;
    bool m_relationalDatabaseNameHasBeenSet = false;
  };

} // namespace Model
} // namespace Lightsail
} // namespace Aws
