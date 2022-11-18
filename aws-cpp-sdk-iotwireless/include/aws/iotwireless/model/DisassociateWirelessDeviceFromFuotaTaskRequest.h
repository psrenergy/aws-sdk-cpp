﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/iotwireless/IoTWireless_EXPORTS.h>
#include <aws/iotwireless/IoTWirelessRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace IoTWireless
{
namespace Model
{

  /**
   */
  class AWS_IOTWIRELESS_API DisassociateWirelessDeviceFromFuotaTaskRequest : public IoTWirelessRequest
  {
  public:
    DisassociateWirelessDeviceFromFuotaTaskRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DisassociateWirelessDeviceFromFuotaTaskRequest> Clone() const
    {
      return Aws::MakeUnique<DisassociateWirelessDeviceFromFuotaTaskRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DisassociateWirelessDeviceFromFuotaTask"; }

    Aws::String SerializePayload() const override;


    
    inline const Aws::String& GetId() const{ return m_id; }

    
    inline bool IdHasBeenSet() const { return m_idHasBeenSet; }

    
    inline void SetId(const Aws::String& value) { m_idHasBeenSet = true; m_id = value; }

    
    inline void SetId(Aws::String&& value) { m_idHasBeenSet = true; m_id = std::move(value); }

    
    inline void SetId(const char* value) { m_idHasBeenSet = true; m_id.assign(value); }

    
    inline DisassociateWirelessDeviceFromFuotaTaskRequest& WithId(const Aws::String& value) { SetId(value); return *this;}

    
    inline DisassociateWirelessDeviceFromFuotaTaskRequest& WithId(Aws::String&& value) { SetId(std::move(value)); return *this;}

    
    inline DisassociateWirelessDeviceFromFuotaTaskRequest& WithId(const char* value) { SetId(value); return *this;}


    
    inline const Aws::String& GetWirelessDeviceId() const{ return m_wirelessDeviceId; }

    
    inline bool WirelessDeviceIdHasBeenSet() const { return m_wirelessDeviceIdHasBeenSet; }

    
    inline void SetWirelessDeviceId(const Aws::String& value) { m_wirelessDeviceIdHasBeenSet = true; m_wirelessDeviceId = value; }

    
    inline void SetWirelessDeviceId(Aws::String&& value) { m_wirelessDeviceIdHasBeenSet = true; m_wirelessDeviceId = std::move(value); }

    
    inline void SetWirelessDeviceId(const char* value) { m_wirelessDeviceIdHasBeenSet = true; m_wirelessDeviceId.assign(value); }

    
    inline DisassociateWirelessDeviceFromFuotaTaskRequest& WithWirelessDeviceId(const Aws::String& value) { SetWirelessDeviceId(value); return *this;}

    
    inline DisassociateWirelessDeviceFromFuotaTaskRequest& WithWirelessDeviceId(Aws::String&& value) { SetWirelessDeviceId(std::move(value)); return *this;}

    
    inline DisassociateWirelessDeviceFromFuotaTaskRequest& WithWirelessDeviceId(const char* value) { SetWirelessDeviceId(value); return *this;}

  private:

    Aws::String m_id;
    bool m_idHasBeenSet = false;

    Aws::String m_wirelessDeviceId;
    bool m_wirelessDeviceIdHasBeenSet = false;
  };

} // namespace Model
} // namespace IoTWireless
} // namespace Aws
