﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/network-firewall/NetworkFirewall_EXPORTS.h>
#include <aws/network-firewall/NetworkFirewallRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <utility>

namespace Aws
{
namespace NetworkFirewall
{
namespace Model
{

  /**
   */
  class AWS_NETWORKFIREWALL_API DisassociateSubnetsRequest : public NetworkFirewallRequest
  {
  public:
    DisassociateSubnetsRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DisassociateSubnetsRequest> Clone() const
    {
      return Aws::MakeUnique<DisassociateSubnetsRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DisassociateSubnets"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>An optional token that you can use for optimistic locking. Network Firewall
     * returns a token to your requests that access the firewall. The token marks the
     * state of the firewall resource at the time of the request. </p> <p>To make an
     * unconditional change to the firewall, omit the token in your update request.
     * Without the token, Network Firewall performs your updates regardless of whether
     * the firewall has changed since you last retrieved it.</p> <p>To make a
     * conditional change to the firewall, provide the token in your update request.
     * Network Firewall uses the token to ensure that the firewall hasn't changed since
     * you last retrieved it. If it has changed, the operation fails with an
     * <code>InvalidTokenException</code>. If this happens, retrieve the firewall again
     * to get a current copy of it with a new token. Reapply your changes as needed,
     * then try the operation again using the new token. </p>
     */
    inline const Aws::String& GetUpdateToken() const{ return m_updateToken; }

    /**
     * <p>An optional token that you can use for optimistic locking. Network Firewall
     * returns a token to your requests that access the firewall. The token marks the
     * state of the firewall resource at the time of the request. </p> <p>To make an
     * unconditional change to the firewall, omit the token in your update request.
     * Without the token, Network Firewall performs your updates regardless of whether
     * the firewall has changed since you last retrieved it.</p> <p>To make a
     * conditional change to the firewall, provide the token in your update request.
     * Network Firewall uses the token to ensure that the firewall hasn't changed since
     * you last retrieved it. If it has changed, the operation fails with an
     * <code>InvalidTokenException</code>. If this happens, retrieve the firewall again
     * to get a current copy of it with a new token. Reapply your changes as needed,
     * then try the operation again using the new token. </p>
     */
    inline bool UpdateTokenHasBeenSet() const { return m_updateTokenHasBeenSet; }

    /**
     * <p>An optional token that you can use for optimistic locking. Network Firewall
     * returns a token to your requests that access the firewall. The token marks the
     * state of the firewall resource at the time of the request. </p> <p>To make an
     * unconditional change to the firewall, omit the token in your update request.
     * Without the token, Network Firewall performs your updates regardless of whether
     * the firewall has changed since you last retrieved it.</p> <p>To make a
     * conditional change to the firewall, provide the token in your update request.
     * Network Firewall uses the token to ensure that the firewall hasn't changed since
     * you last retrieved it. If it has changed, the operation fails with an
     * <code>InvalidTokenException</code>. If this happens, retrieve the firewall again
     * to get a current copy of it with a new token. Reapply your changes as needed,
     * then try the operation again using the new token. </p>
     */
    inline void SetUpdateToken(const Aws::String& value) { m_updateTokenHasBeenSet = true; m_updateToken = value; }

    /**
     * <p>An optional token that you can use for optimistic locking. Network Firewall
     * returns a token to your requests that access the firewall. The token marks the
     * state of the firewall resource at the time of the request. </p> <p>To make an
     * unconditional change to the firewall, omit the token in your update request.
     * Without the token, Network Firewall performs your updates regardless of whether
     * the firewall has changed since you last retrieved it.</p> <p>To make a
     * conditional change to the firewall, provide the token in your update request.
     * Network Firewall uses the token to ensure that the firewall hasn't changed since
     * you last retrieved it. If it has changed, the operation fails with an
     * <code>InvalidTokenException</code>. If this happens, retrieve the firewall again
     * to get a current copy of it with a new token. Reapply your changes as needed,
     * then try the operation again using the new token. </p>
     */
    inline void SetUpdateToken(Aws::String&& value) { m_updateTokenHasBeenSet = true; m_updateToken = std::move(value); }

    /**
     * <p>An optional token that you can use for optimistic locking. Network Firewall
     * returns a token to your requests that access the firewall. The token marks the
     * state of the firewall resource at the time of the request. </p> <p>To make an
     * unconditional change to the firewall, omit the token in your update request.
     * Without the token, Network Firewall performs your updates regardless of whether
     * the firewall has changed since you last retrieved it.</p> <p>To make a
     * conditional change to the firewall, provide the token in your update request.
     * Network Firewall uses the token to ensure that the firewall hasn't changed since
     * you last retrieved it. If it has changed, the operation fails with an
     * <code>InvalidTokenException</code>. If this happens, retrieve the firewall again
     * to get a current copy of it with a new token. Reapply your changes as needed,
     * then try the operation again using the new token. </p>
     */
    inline void SetUpdateToken(const char* value) { m_updateTokenHasBeenSet = true; m_updateToken.assign(value); }

    /**
     * <p>An optional token that you can use for optimistic locking. Network Firewall
     * returns a token to your requests that access the firewall. The token marks the
     * state of the firewall resource at the time of the request. </p> <p>To make an
     * unconditional change to the firewall, omit the token in your update request.
     * Without the token, Network Firewall performs your updates regardless of whether
     * the firewall has changed since you last retrieved it.</p> <p>To make a
     * conditional change to the firewall, provide the token in your update request.
     * Network Firewall uses the token to ensure that the firewall hasn't changed since
     * you last retrieved it. If it has changed, the operation fails with an
     * <code>InvalidTokenException</code>. If this happens, retrieve the firewall again
     * to get a current copy of it with a new token. Reapply your changes as needed,
     * then try the operation again using the new token. </p>
     */
    inline DisassociateSubnetsRequest& WithUpdateToken(const Aws::String& value) { SetUpdateToken(value); return *this;}

    /**
     * <p>An optional token that you can use for optimistic locking. Network Firewall
     * returns a token to your requests that access the firewall. The token marks the
     * state of the firewall resource at the time of the request. </p> <p>To make an
     * unconditional change to the firewall, omit the token in your update request.
     * Without the token, Network Firewall performs your updates regardless of whether
     * the firewall has changed since you last retrieved it.</p> <p>To make a
     * conditional change to the firewall, provide the token in your update request.
     * Network Firewall uses the token to ensure that the firewall hasn't changed since
     * you last retrieved it. If it has changed, the operation fails with an
     * <code>InvalidTokenException</code>. If this happens, retrieve the firewall again
     * to get a current copy of it with a new token. Reapply your changes as needed,
     * then try the operation again using the new token. </p>
     */
    inline DisassociateSubnetsRequest& WithUpdateToken(Aws::String&& value) { SetUpdateToken(std::move(value)); return *this;}

    /**
     * <p>An optional token that you can use for optimistic locking. Network Firewall
     * returns a token to your requests that access the firewall. The token marks the
     * state of the firewall resource at the time of the request. </p> <p>To make an
     * unconditional change to the firewall, omit the token in your update request.
     * Without the token, Network Firewall performs your updates regardless of whether
     * the firewall has changed since you last retrieved it.</p> <p>To make a
     * conditional change to the firewall, provide the token in your update request.
     * Network Firewall uses the token to ensure that the firewall hasn't changed since
     * you last retrieved it. If it has changed, the operation fails with an
     * <code>InvalidTokenException</code>. If this happens, retrieve the firewall again
     * to get a current copy of it with a new token. Reapply your changes as needed,
     * then try the operation again using the new token. </p>
     */
    inline DisassociateSubnetsRequest& WithUpdateToken(const char* value) { SetUpdateToken(value); return *this;}


    /**
     * <p>The Amazon Resource Name (ARN) of the firewall.</p> <p>You must specify the
     * ARN or the name, and you can specify both. </p>
     */
    inline const Aws::String& GetFirewallArn() const{ return m_firewallArn; }

    /**
     * <p>The Amazon Resource Name (ARN) of the firewall.</p> <p>You must specify the
     * ARN or the name, and you can specify both. </p>
     */
    inline bool FirewallArnHasBeenSet() const { return m_firewallArnHasBeenSet; }

    /**
     * <p>The Amazon Resource Name (ARN) of the firewall.</p> <p>You must specify the
     * ARN or the name, and you can specify both. </p>
     */
    inline void SetFirewallArn(const Aws::String& value) { m_firewallArnHasBeenSet = true; m_firewallArn = value; }

    /**
     * <p>The Amazon Resource Name (ARN) of the firewall.</p> <p>You must specify the
     * ARN or the name, and you can specify both. </p>
     */
    inline void SetFirewallArn(Aws::String&& value) { m_firewallArnHasBeenSet = true; m_firewallArn = std::move(value); }

    /**
     * <p>The Amazon Resource Name (ARN) of the firewall.</p> <p>You must specify the
     * ARN or the name, and you can specify both. </p>
     */
    inline void SetFirewallArn(const char* value) { m_firewallArnHasBeenSet = true; m_firewallArn.assign(value); }

    /**
     * <p>The Amazon Resource Name (ARN) of the firewall.</p> <p>You must specify the
     * ARN or the name, and you can specify both. </p>
     */
    inline DisassociateSubnetsRequest& WithFirewallArn(const Aws::String& value) { SetFirewallArn(value); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) of the firewall.</p> <p>You must specify the
     * ARN or the name, and you can specify both. </p>
     */
    inline DisassociateSubnetsRequest& WithFirewallArn(Aws::String&& value) { SetFirewallArn(std::move(value)); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) of the firewall.</p> <p>You must specify the
     * ARN or the name, and you can specify both. </p>
     */
    inline DisassociateSubnetsRequest& WithFirewallArn(const char* value) { SetFirewallArn(value); return *this;}


    /**
     * <p>The descriptive name of the firewall. You can't change the name of a firewall
     * after you create it.</p> <p>You must specify the ARN or the name, and you can
     * specify both. </p>
     */
    inline const Aws::String& GetFirewallName() const{ return m_firewallName; }

    /**
     * <p>The descriptive name of the firewall. You can't change the name of a firewall
     * after you create it.</p> <p>You must specify the ARN or the name, and you can
     * specify both. </p>
     */
    inline bool FirewallNameHasBeenSet() const { return m_firewallNameHasBeenSet; }

    /**
     * <p>The descriptive name of the firewall. You can't change the name of a firewall
     * after you create it.</p> <p>You must specify the ARN or the name, and you can
     * specify both. </p>
     */
    inline void SetFirewallName(const Aws::String& value) { m_firewallNameHasBeenSet = true; m_firewallName = value; }

    /**
     * <p>The descriptive name of the firewall. You can't change the name of a firewall
     * after you create it.</p> <p>You must specify the ARN or the name, and you can
     * specify both. </p>
     */
    inline void SetFirewallName(Aws::String&& value) { m_firewallNameHasBeenSet = true; m_firewallName = std::move(value); }

    /**
     * <p>The descriptive name of the firewall. You can't change the name of a firewall
     * after you create it.</p> <p>You must specify the ARN or the name, and you can
     * specify both. </p>
     */
    inline void SetFirewallName(const char* value) { m_firewallNameHasBeenSet = true; m_firewallName.assign(value); }

    /**
     * <p>The descriptive name of the firewall. You can't change the name of a firewall
     * after you create it.</p> <p>You must specify the ARN or the name, and you can
     * specify both. </p>
     */
    inline DisassociateSubnetsRequest& WithFirewallName(const Aws::String& value) { SetFirewallName(value); return *this;}

    /**
     * <p>The descriptive name of the firewall. You can't change the name of a firewall
     * after you create it.</p> <p>You must specify the ARN or the name, and you can
     * specify both. </p>
     */
    inline DisassociateSubnetsRequest& WithFirewallName(Aws::String&& value) { SetFirewallName(std::move(value)); return *this;}

    /**
     * <p>The descriptive name of the firewall. You can't change the name of a firewall
     * after you create it.</p> <p>You must specify the ARN or the name, and you can
     * specify both. </p>
     */
    inline DisassociateSubnetsRequest& WithFirewallName(const char* value) { SetFirewallName(value); return *this;}


    /**
     * <p>The unique identifiers for the subnets that you want to disassociate. </p>
     */
    inline const Aws::Vector<Aws::String>& GetSubnetIds() const{ return m_subnetIds; }

    /**
     * <p>The unique identifiers for the subnets that you want to disassociate. </p>
     */
    inline bool SubnetIdsHasBeenSet() const { return m_subnetIdsHasBeenSet; }

    /**
     * <p>The unique identifiers for the subnets that you want to disassociate. </p>
     */
    inline void SetSubnetIds(const Aws::Vector<Aws::String>& value) { m_subnetIdsHasBeenSet = true; m_subnetIds = value; }

    /**
     * <p>The unique identifiers for the subnets that you want to disassociate. </p>
     */
    inline void SetSubnetIds(Aws::Vector<Aws::String>&& value) { m_subnetIdsHasBeenSet = true; m_subnetIds = std::move(value); }

    /**
     * <p>The unique identifiers for the subnets that you want to disassociate. </p>
     */
    inline DisassociateSubnetsRequest& WithSubnetIds(const Aws::Vector<Aws::String>& value) { SetSubnetIds(value); return *this;}

    /**
     * <p>The unique identifiers for the subnets that you want to disassociate. </p>
     */
    inline DisassociateSubnetsRequest& WithSubnetIds(Aws::Vector<Aws::String>&& value) { SetSubnetIds(std::move(value)); return *this;}

    /**
     * <p>The unique identifiers for the subnets that you want to disassociate. </p>
     */
    inline DisassociateSubnetsRequest& AddSubnetIds(const Aws::String& value) { m_subnetIdsHasBeenSet = true; m_subnetIds.push_back(value); return *this; }

    /**
     * <p>The unique identifiers for the subnets that you want to disassociate. </p>
     */
    inline DisassociateSubnetsRequest& AddSubnetIds(Aws::String&& value) { m_subnetIdsHasBeenSet = true; m_subnetIds.push_back(std::move(value)); return *this; }

    /**
     * <p>The unique identifiers for the subnets that you want to disassociate. </p>
     */
    inline DisassociateSubnetsRequest& AddSubnetIds(const char* value) { m_subnetIdsHasBeenSet = true; m_subnetIds.push_back(value); return *this; }

  private:

    Aws::String m_updateToken;
    bool m_updateTokenHasBeenSet = false;

    Aws::String m_firewallArn;
    bool m_firewallArnHasBeenSet = false;

    Aws::String m_firewallName;
    bool m_firewallNameHasBeenSet = false;

    Aws::Vector<Aws::String> m_subnetIds;
    bool m_subnetIdsHasBeenSet = false;
  };

} // namespace Model
} // namespace NetworkFirewall
} // namespace Aws
