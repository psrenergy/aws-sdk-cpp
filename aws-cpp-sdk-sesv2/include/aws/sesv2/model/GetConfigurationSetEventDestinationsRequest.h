﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/sesv2/SESV2_EXPORTS.h>
#include <aws/sesv2/SESV2Request.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace SESV2
{
namespace Model
{

  /**
   * <p>A request to obtain information about the event destinations for a
   * configuration set.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/sesv2-2019-09-27/GetConfigurationSetEventDestinationsRequest">AWS
   * API Reference</a></p>
   */
  class AWS_SESV2_API GetConfigurationSetEventDestinationsRequest : public SESV2Request
  {
  public:
    GetConfigurationSetEventDestinationsRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<GetConfigurationSetEventDestinationsRequest> Clone() const
    {
      return Aws::MakeUnique<GetConfigurationSetEventDestinationsRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetConfigurationSetEventDestinations"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The name of the configuration set that contains the event destination.</p>
     */
    inline const Aws::String& GetConfigurationSetName() const{ return m_configurationSetName; }

    /**
     * <p>The name of the configuration set that contains the event destination.</p>
     */
    inline bool ConfigurationSetNameHasBeenSet() const { return m_configurationSetNameHasBeenSet; }

    /**
     * <p>The name of the configuration set that contains the event destination.</p>
     */
    inline void SetConfigurationSetName(const Aws::String& value) { m_configurationSetNameHasBeenSet = true; m_configurationSetName = value; }

    /**
     * <p>The name of the configuration set that contains the event destination.</p>
     */
    inline void SetConfigurationSetName(Aws::String&& value) { m_configurationSetNameHasBeenSet = true; m_configurationSetName = std::move(value); }

    /**
     * <p>The name of the configuration set that contains the event destination.</p>
     */
    inline void SetConfigurationSetName(const char* value) { m_configurationSetNameHasBeenSet = true; m_configurationSetName.assign(value); }

    /**
     * <p>The name of the configuration set that contains the event destination.</p>
     */
    inline GetConfigurationSetEventDestinationsRequest& WithConfigurationSetName(const Aws::String& value) { SetConfigurationSetName(value); return *this;}

    /**
     * <p>The name of the configuration set that contains the event destination.</p>
     */
    inline GetConfigurationSetEventDestinationsRequest& WithConfigurationSetName(Aws::String&& value) { SetConfigurationSetName(std::move(value)); return *this;}

    /**
     * <p>The name of the configuration set that contains the event destination.</p>
     */
    inline GetConfigurationSetEventDestinationsRequest& WithConfigurationSetName(const char* value) { SetConfigurationSetName(value); return *this;}

  private:

    Aws::String m_configurationSetName;
    bool m_configurationSetNameHasBeenSet = false;
  };

} // namespace Model
} // namespace SESV2
} // namespace Aws
