﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/es/ElasticsearchService_EXPORTS.h>
#include <aws/es/ElasticsearchServiceRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace ElasticsearchService
{
namespace Model
{

  /**
   * <p>Container for the parameters to the
   * <code><a>RejectInboundCrossClusterSearchConnection</a></code>
   * operation.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/es-2015-01-01/RejectInboundCrossClusterSearchConnectionRequest">AWS
   * API Reference</a></p>
   */
  class AWS_ELASTICSEARCHSERVICE_API RejectInboundCrossClusterSearchConnectionRequest : public ElasticsearchServiceRequest
  {
  public:
    RejectInboundCrossClusterSearchConnectionRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<RejectInboundCrossClusterSearchConnectionRequest> Clone() const
    {
      return Aws::MakeUnique<RejectInboundCrossClusterSearchConnectionRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "RejectInboundCrossClusterSearchConnection"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The id of the inbound connection that you want to reject.</p>
     */
    inline const Aws::String& GetCrossClusterSearchConnectionId() const{ return m_crossClusterSearchConnectionId; }

    /**
     * <p>The id of the inbound connection that you want to reject.</p>
     */
    inline bool CrossClusterSearchConnectionIdHasBeenSet() const { return m_crossClusterSearchConnectionIdHasBeenSet; }

    /**
     * <p>The id of the inbound connection that you want to reject.</p>
     */
    inline void SetCrossClusterSearchConnectionId(const Aws::String& value) { m_crossClusterSearchConnectionIdHasBeenSet = true; m_crossClusterSearchConnectionId = value; }

    /**
     * <p>The id of the inbound connection that you want to reject.</p>
     */
    inline void SetCrossClusterSearchConnectionId(Aws::String&& value) { m_crossClusterSearchConnectionIdHasBeenSet = true; m_crossClusterSearchConnectionId = std::move(value); }

    /**
     * <p>The id of the inbound connection that you want to reject.</p>
     */
    inline void SetCrossClusterSearchConnectionId(const char* value) { m_crossClusterSearchConnectionIdHasBeenSet = true; m_crossClusterSearchConnectionId.assign(value); }

    /**
     * <p>The id of the inbound connection that you want to reject.</p>
     */
    inline RejectInboundCrossClusterSearchConnectionRequest& WithCrossClusterSearchConnectionId(const Aws::String& value) { SetCrossClusterSearchConnectionId(value); return *this;}

    /**
     * <p>The id of the inbound connection that you want to reject.</p>
     */
    inline RejectInboundCrossClusterSearchConnectionRequest& WithCrossClusterSearchConnectionId(Aws::String&& value) { SetCrossClusterSearchConnectionId(std::move(value)); return *this;}

    /**
     * <p>The id of the inbound connection that you want to reject.</p>
     */
    inline RejectInboundCrossClusterSearchConnectionRequest& WithCrossClusterSearchConnectionId(const char* value) { SetCrossClusterSearchConnectionId(value); return *this;}

  private:

    Aws::String m_crossClusterSearchConnectionId;
    bool m_crossClusterSearchConnectionIdHasBeenSet = false;
  };

} // namespace Model
} // namespace ElasticsearchService
} // namespace Aws
