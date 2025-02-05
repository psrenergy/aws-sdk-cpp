﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/sesv2/SESV2_EXPORTS.h>
#include <aws/sesv2/SESV2Request.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/sesv2/model/EventDestinationDefinition.h>
#include <utility>

namespace Aws
{
namespace SESV2
{
namespace Model
{

  /**
   * <p>A request to change the settings for an event destination for a configuration
   * set.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/sesv2-2019-09-27/UpdateConfigurationSetEventDestinationRequest">AWS
   * API Reference</a></p>
   */
  class AWS_SESV2_API UpdateConfigurationSetEventDestinationRequest : public SESV2Request
  {
  public:
    UpdateConfigurationSetEventDestinationRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "UpdateConfigurationSetEventDestination"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The name of the configuration set that contains the event destination to
     * modify.</p>
     */
    inline const Aws::String& GetConfigurationSetName() const{ return m_configurationSetName; }

    /**
     * <p>The name of the configuration set that contains the event destination to
     * modify.</p>
     */
    inline bool ConfigurationSetNameHasBeenSet() const { return m_configurationSetNameHasBeenSet; }

    /**
     * <p>The name of the configuration set that contains the event destination to
     * modify.</p>
     */
    inline void SetConfigurationSetName(const Aws::String& value) { m_configurationSetNameHasBeenSet = true; m_configurationSetName = value; }

    /**
     * <p>The name of the configuration set that contains the event destination to
     * modify.</p>
     */
    inline void SetConfigurationSetName(Aws::String&& value) { m_configurationSetNameHasBeenSet = true; m_configurationSetName = std::move(value); }

    /**
     * <p>The name of the configuration set that contains the event destination to
     * modify.</p>
     */
    inline void SetConfigurationSetName(const char* value) { m_configurationSetNameHasBeenSet = true; m_configurationSetName.assign(value); }

    /**
     * <p>The name of the configuration set that contains the event destination to
     * modify.</p>
     */
    inline UpdateConfigurationSetEventDestinationRequest& WithConfigurationSetName(const Aws::String& value) { SetConfigurationSetName(value); return *this;}

    /**
     * <p>The name of the configuration set that contains the event destination to
     * modify.</p>
     */
    inline UpdateConfigurationSetEventDestinationRequest& WithConfigurationSetName(Aws::String&& value) { SetConfigurationSetName(std::move(value)); return *this;}

    /**
     * <p>The name of the configuration set that contains the event destination to
     * modify.</p>
     */
    inline UpdateConfigurationSetEventDestinationRequest& WithConfigurationSetName(const char* value) { SetConfigurationSetName(value); return *this;}


    /**
     * <p>The name of the event destination.</p>
     */
    inline const Aws::String& GetEventDestinationName() const{ return m_eventDestinationName; }

    /**
     * <p>The name of the event destination.</p>
     */
    inline bool EventDestinationNameHasBeenSet() const { return m_eventDestinationNameHasBeenSet; }

    /**
     * <p>The name of the event destination.</p>
     */
    inline void SetEventDestinationName(const Aws::String& value) { m_eventDestinationNameHasBeenSet = true; m_eventDestinationName = value; }

    /**
     * <p>The name of the event destination.</p>
     */
    inline void SetEventDestinationName(Aws::String&& value) { m_eventDestinationNameHasBeenSet = true; m_eventDestinationName = std::move(value); }

    /**
     * <p>The name of the event destination.</p>
     */
    inline void SetEventDestinationName(const char* value) { m_eventDestinationNameHasBeenSet = true; m_eventDestinationName.assign(value); }

    /**
     * <p>The name of the event destination.</p>
     */
    inline UpdateConfigurationSetEventDestinationRequest& WithEventDestinationName(const Aws::String& value) { SetEventDestinationName(value); return *this;}

    /**
     * <p>The name of the event destination.</p>
     */
    inline UpdateConfigurationSetEventDestinationRequest& WithEventDestinationName(Aws::String&& value) { SetEventDestinationName(std::move(value)); return *this;}

    /**
     * <p>The name of the event destination.</p>
     */
    inline UpdateConfigurationSetEventDestinationRequest& WithEventDestinationName(const char* value) { SetEventDestinationName(value); return *this;}


    /**
     * <p>An object that defines the event destination.</p>
     */
    inline const EventDestinationDefinition& GetEventDestination() const{ return m_eventDestination; }

    /**
     * <p>An object that defines the event destination.</p>
     */
    inline bool EventDestinationHasBeenSet() const { return m_eventDestinationHasBeenSet; }

    /**
     * <p>An object that defines the event destination.</p>
     */
    inline void SetEventDestination(const EventDestinationDefinition& value) { m_eventDestinationHasBeenSet = true; m_eventDestination = value; }

    /**
     * <p>An object that defines the event destination.</p>
     */
    inline void SetEventDestination(EventDestinationDefinition&& value) { m_eventDestinationHasBeenSet = true; m_eventDestination = std::move(value); }

    /**
     * <p>An object that defines the event destination.</p>
     */
    inline UpdateConfigurationSetEventDestinationRequest& WithEventDestination(const EventDestinationDefinition& value) { SetEventDestination(value); return *this;}

    /**
     * <p>An object that defines the event destination.</p>
     */
    inline UpdateConfigurationSetEventDestinationRequest& WithEventDestination(EventDestinationDefinition&& value) { SetEventDestination(std::move(value)); return *this;}

  private:

    Aws::String m_configurationSetName;
    bool m_configurationSetNameHasBeenSet = false;

    Aws::String m_eventDestinationName;
    bool m_eventDestinationNameHasBeenSet = false;

    EventDestinationDefinition m_eventDestination;
    bool m_eventDestinationHasBeenSet = false;
  };

} // namespace Model
} // namespace SESV2
} // namespace Aws
