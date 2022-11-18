﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/awstransfer/Transfer_EXPORTS.h>
#include <aws/awstransfer/TransferRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Transfer
{
namespace Model
{

  /**
   */
  class AWS_TRANSFER_API StopServerRequest : public TransferRequest
  {
  public:
    StopServerRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<StopServerRequest> Clone() const
    {
      return Aws::MakeUnique<StopServerRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "StopServer"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>A system-assigned unique identifier for a server that you stopped.</p>
     */
    inline const Aws::String& GetServerId() const{ return m_serverId; }

    /**
     * <p>A system-assigned unique identifier for a server that you stopped.</p>
     */
    inline bool ServerIdHasBeenSet() const { return m_serverIdHasBeenSet; }

    /**
     * <p>A system-assigned unique identifier for a server that you stopped.</p>
     */
    inline void SetServerId(const Aws::String& value) { m_serverIdHasBeenSet = true; m_serverId = value; }

    /**
     * <p>A system-assigned unique identifier for a server that you stopped.</p>
     */
    inline void SetServerId(Aws::String&& value) { m_serverIdHasBeenSet = true; m_serverId = std::move(value); }

    /**
     * <p>A system-assigned unique identifier for a server that you stopped.</p>
     */
    inline void SetServerId(const char* value) { m_serverIdHasBeenSet = true; m_serverId.assign(value); }

    /**
     * <p>A system-assigned unique identifier for a server that you stopped.</p>
     */
    inline StopServerRequest& WithServerId(const Aws::String& value) { SetServerId(value); return *this;}

    /**
     * <p>A system-assigned unique identifier for a server that you stopped.</p>
     */
    inline StopServerRequest& WithServerId(Aws::String&& value) { SetServerId(std::move(value)); return *this;}

    /**
     * <p>A system-assigned unique identifier for a server that you stopped.</p>
     */
    inline StopServerRequest& WithServerId(const char* value) { SetServerId(value); return *this;}

  private:

    Aws::String m_serverId;
    bool m_serverIdHasBeenSet = false;
  };

} // namespace Model
} // namespace Transfer
} // namespace Aws
