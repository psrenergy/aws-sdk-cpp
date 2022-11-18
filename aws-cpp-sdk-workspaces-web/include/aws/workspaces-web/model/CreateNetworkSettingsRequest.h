﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/workspaces-web/WorkSpacesWeb_EXPORTS.h>
#include <aws/workspaces-web/WorkSpacesWebRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/workspaces-web/model/Tag.h>
#include <utility>
#include <aws/core/utils/UUID.h>

namespace Aws
{
namespace WorkSpacesWeb
{
namespace Model
{

  /**
   */
  class AWS_WORKSPACESWEB_API CreateNetworkSettingsRequest : public WorkSpacesWebRequest
  {
  public:
    CreateNetworkSettingsRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<CreateNetworkSettingsRequest> Clone() const
    {
      return Aws::MakeUnique<CreateNetworkSettingsRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "CreateNetworkSettings"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>A unique, case-sensitive identifier that you provide to ensure the
     * idempotency of the request. Idempotency ensures that an API request completes
     * only once. With an idempotent request, if the original request completes
     * successfully, subsequent retries with the same client token returns the result
     * from the original successful request. </p> <p>If you do not specify a client
     * token, one is automatically generated by the AWS SDK.</p>
     */
    inline const Aws::String& GetClientToken() const{ return m_clientToken; }

    /**
     * <p>A unique, case-sensitive identifier that you provide to ensure the
     * idempotency of the request. Idempotency ensures that an API request completes
     * only once. With an idempotent request, if the original request completes
     * successfully, subsequent retries with the same client token returns the result
     * from the original successful request. </p> <p>If you do not specify a client
     * token, one is automatically generated by the AWS SDK.</p>
     */
    inline bool ClientTokenHasBeenSet() const { return m_clientTokenHasBeenSet; }

    /**
     * <p>A unique, case-sensitive identifier that you provide to ensure the
     * idempotency of the request. Idempotency ensures that an API request completes
     * only once. With an idempotent request, if the original request completes
     * successfully, subsequent retries with the same client token returns the result
     * from the original successful request. </p> <p>If you do not specify a client
     * token, one is automatically generated by the AWS SDK.</p>
     */
    inline void SetClientToken(const Aws::String& value) { m_clientTokenHasBeenSet = true; m_clientToken = value; }

    /**
     * <p>A unique, case-sensitive identifier that you provide to ensure the
     * idempotency of the request. Idempotency ensures that an API request completes
     * only once. With an idempotent request, if the original request completes
     * successfully, subsequent retries with the same client token returns the result
     * from the original successful request. </p> <p>If you do not specify a client
     * token, one is automatically generated by the AWS SDK.</p>
     */
    inline void SetClientToken(Aws::String&& value) { m_clientTokenHasBeenSet = true; m_clientToken = std::move(value); }

    /**
     * <p>A unique, case-sensitive identifier that you provide to ensure the
     * idempotency of the request. Idempotency ensures that an API request completes
     * only once. With an idempotent request, if the original request completes
     * successfully, subsequent retries with the same client token returns the result
     * from the original successful request. </p> <p>If you do not specify a client
     * token, one is automatically generated by the AWS SDK.</p>
     */
    inline void SetClientToken(const char* value) { m_clientTokenHasBeenSet = true; m_clientToken.assign(value); }

    /**
     * <p>A unique, case-sensitive identifier that you provide to ensure the
     * idempotency of the request. Idempotency ensures that an API request completes
     * only once. With an idempotent request, if the original request completes
     * successfully, subsequent retries with the same client token returns the result
     * from the original successful request. </p> <p>If you do not specify a client
     * token, one is automatically generated by the AWS SDK.</p>
     */
    inline CreateNetworkSettingsRequest& WithClientToken(const Aws::String& value) { SetClientToken(value); return *this;}

    /**
     * <p>A unique, case-sensitive identifier that you provide to ensure the
     * idempotency of the request. Idempotency ensures that an API request completes
     * only once. With an idempotent request, if the original request completes
     * successfully, subsequent retries with the same client token returns the result
     * from the original successful request. </p> <p>If you do not specify a client
     * token, one is automatically generated by the AWS SDK.</p>
     */
    inline CreateNetworkSettingsRequest& WithClientToken(Aws::String&& value) { SetClientToken(std::move(value)); return *this;}

    /**
     * <p>A unique, case-sensitive identifier that you provide to ensure the
     * idempotency of the request. Idempotency ensures that an API request completes
     * only once. With an idempotent request, if the original request completes
     * successfully, subsequent retries with the same client token returns the result
     * from the original successful request. </p> <p>If you do not specify a client
     * token, one is automatically generated by the AWS SDK.</p>
     */
    inline CreateNetworkSettingsRequest& WithClientToken(const char* value) { SetClientToken(value); return *this;}


    /**
     * <p>One or more security groups used to control access from streaming instances
     * to your VPC.</p>
     */
    inline const Aws::Vector<Aws::String>& GetSecurityGroupIds() const{ return m_securityGroupIds; }

    /**
     * <p>One or more security groups used to control access from streaming instances
     * to your VPC.</p>
     */
    inline bool SecurityGroupIdsHasBeenSet() const { return m_securityGroupIdsHasBeenSet; }

    /**
     * <p>One or more security groups used to control access from streaming instances
     * to your VPC.</p>
     */
    inline void SetSecurityGroupIds(const Aws::Vector<Aws::String>& value) { m_securityGroupIdsHasBeenSet = true; m_securityGroupIds = value; }

    /**
     * <p>One or more security groups used to control access from streaming instances
     * to your VPC.</p>
     */
    inline void SetSecurityGroupIds(Aws::Vector<Aws::String>&& value) { m_securityGroupIdsHasBeenSet = true; m_securityGroupIds = std::move(value); }

    /**
     * <p>One or more security groups used to control access from streaming instances
     * to your VPC.</p>
     */
    inline CreateNetworkSettingsRequest& WithSecurityGroupIds(const Aws::Vector<Aws::String>& value) { SetSecurityGroupIds(value); return *this;}

    /**
     * <p>One or more security groups used to control access from streaming instances
     * to your VPC.</p>
     */
    inline CreateNetworkSettingsRequest& WithSecurityGroupIds(Aws::Vector<Aws::String>&& value) { SetSecurityGroupIds(std::move(value)); return *this;}

    /**
     * <p>One or more security groups used to control access from streaming instances
     * to your VPC.</p>
     */
    inline CreateNetworkSettingsRequest& AddSecurityGroupIds(const Aws::String& value) { m_securityGroupIdsHasBeenSet = true; m_securityGroupIds.push_back(value); return *this; }

    /**
     * <p>One or more security groups used to control access from streaming instances
     * to your VPC.</p>
     */
    inline CreateNetworkSettingsRequest& AddSecurityGroupIds(Aws::String&& value) { m_securityGroupIdsHasBeenSet = true; m_securityGroupIds.push_back(std::move(value)); return *this; }

    /**
     * <p>One or more security groups used to control access from streaming instances
     * to your VPC.</p>
     */
    inline CreateNetworkSettingsRequest& AddSecurityGroupIds(const char* value) { m_securityGroupIdsHasBeenSet = true; m_securityGroupIds.push_back(value); return *this; }


    /**
     * <p>The subnets in which network interfaces are created to connect streaming
     * instances to your VPC. At least two of these subnets must be in different
     * availability zones.</p>
     */
    inline const Aws::Vector<Aws::String>& GetSubnetIds() const{ return m_subnetIds; }

    /**
     * <p>The subnets in which network interfaces are created to connect streaming
     * instances to your VPC. At least two of these subnets must be in different
     * availability zones.</p>
     */
    inline bool SubnetIdsHasBeenSet() const { return m_subnetIdsHasBeenSet; }

    /**
     * <p>The subnets in which network interfaces are created to connect streaming
     * instances to your VPC. At least two of these subnets must be in different
     * availability zones.</p>
     */
    inline void SetSubnetIds(const Aws::Vector<Aws::String>& value) { m_subnetIdsHasBeenSet = true; m_subnetIds = value; }

    /**
     * <p>The subnets in which network interfaces are created to connect streaming
     * instances to your VPC. At least two of these subnets must be in different
     * availability zones.</p>
     */
    inline void SetSubnetIds(Aws::Vector<Aws::String>&& value) { m_subnetIdsHasBeenSet = true; m_subnetIds = std::move(value); }

    /**
     * <p>The subnets in which network interfaces are created to connect streaming
     * instances to your VPC. At least two of these subnets must be in different
     * availability zones.</p>
     */
    inline CreateNetworkSettingsRequest& WithSubnetIds(const Aws::Vector<Aws::String>& value) { SetSubnetIds(value); return *this;}

    /**
     * <p>The subnets in which network interfaces are created to connect streaming
     * instances to your VPC. At least two of these subnets must be in different
     * availability zones.</p>
     */
    inline CreateNetworkSettingsRequest& WithSubnetIds(Aws::Vector<Aws::String>&& value) { SetSubnetIds(std::move(value)); return *this;}

    /**
     * <p>The subnets in which network interfaces are created to connect streaming
     * instances to your VPC. At least two of these subnets must be in different
     * availability zones.</p>
     */
    inline CreateNetworkSettingsRequest& AddSubnetIds(const Aws::String& value) { m_subnetIdsHasBeenSet = true; m_subnetIds.push_back(value); return *this; }

    /**
     * <p>The subnets in which network interfaces are created to connect streaming
     * instances to your VPC. At least two of these subnets must be in different
     * availability zones.</p>
     */
    inline CreateNetworkSettingsRequest& AddSubnetIds(Aws::String&& value) { m_subnetIdsHasBeenSet = true; m_subnetIds.push_back(std::move(value)); return *this; }

    /**
     * <p>The subnets in which network interfaces are created to connect streaming
     * instances to your VPC. At least two of these subnets must be in different
     * availability zones.</p>
     */
    inline CreateNetworkSettingsRequest& AddSubnetIds(const char* value) { m_subnetIdsHasBeenSet = true; m_subnetIds.push_back(value); return *this; }


    /**
     * <p>The tags to add to the network settings resource. A tag is a key-value
     * pair.</p>
     */
    inline const Aws::Vector<Tag>& GetTags() const{ return m_tags; }

    /**
     * <p>The tags to add to the network settings resource. A tag is a key-value
     * pair.</p>
     */
    inline bool TagsHasBeenSet() const { return m_tagsHasBeenSet; }

    /**
     * <p>The tags to add to the network settings resource. A tag is a key-value
     * pair.</p>
     */
    inline void SetTags(const Aws::Vector<Tag>& value) { m_tagsHasBeenSet = true; m_tags = value; }

    /**
     * <p>The tags to add to the network settings resource. A tag is a key-value
     * pair.</p>
     */
    inline void SetTags(Aws::Vector<Tag>&& value) { m_tagsHasBeenSet = true; m_tags = std::move(value); }

    /**
     * <p>The tags to add to the network settings resource. A tag is a key-value
     * pair.</p>
     */
    inline CreateNetworkSettingsRequest& WithTags(const Aws::Vector<Tag>& value) { SetTags(value); return *this;}

    /**
     * <p>The tags to add to the network settings resource. A tag is a key-value
     * pair.</p>
     */
    inline CreateNetworkSettingsRequest& WithTags(Aws::Vector<Tag>&& value) { SetTags(std::move(value)); return *this;}

    /**
     * <p>The tags to add to the network settings resource. A tag is a key-value
     * pair.</p>
     */
    inline CreateNetworkSettingsRequest& AddTags(const Tag& value) { m_tagsHasBeenSet = true; m_tags.push_back(value); return *this; }

    /**
     * <p>The tags to add to the network settings resource. A tag is a key-value
     * pair.</p>
     */
    inline CreateNetworkSettingsRequest& AddTags(Tag&& value) { m_tagsHasBeenSet = true; m_tags.push_back(std::move(value)); return *this; }


    /**
     * <p>The VPC that streaming instances will connect to.</p>
     */
    inline const Aws::String& GetVpcId() const{ return m_vpcId; }

    /**
     * <p>The VPC that streaming instances will connect to.</p>
     */
    inline bool VpcIdHasBeenSet() const { return m_vpcIdHasBeenSet; }

    /**
     * <p>The VPC that streaming instances will connect to.</p>
     */
    inline void SetVpcId(const Aws::String& value) { m_vpcIdHasBeenSet = true; m_vpcId = value; }

    /**
     * <p>The VPC that streaming instances will connect to.</p>
     */
    inline void SetVpcId(Aws::String&& value) { m_vpcIdHasBeenSet = true; m_vpcId = std::move(value); }

    /**
     * <p>The VPC that streaming instances will connect to.</p>
     */
    inline void SetVpcId(const char* value) { m_vpcIdHasBeenSet = true; m_vpcId.assign(value); }

    /**
     * <p>The VPC that streaming instances will connect to.</p>
     */
    inline CreateNetworkSettingsRequest& WithVpcId(const Aws::String& value) { SetVpcId(value); return *this;}

    /**
     * <p>The VPC that streaming instances will connect to.</p>
     */
    inline CreateNetworkSettingsRequest& WithVpcId(Aws::String&& value) { SetVpcId(std::move(value)); return *this;}

    /**
     * <p>The VPC that streaming instances will connect to.</p>
     */
    inline CreateNetworkSettingsRequest& WithVpcId(const char* value) { SetVpcId(value); return *this;}

  private:

    Aws::String m_clientToken;
    bool m_clientTokenHasBeenSet = false;

    Aws::Vector<Aws::String> m_securityGroupIds;
    bool m_securityGroupIdsHasBeenSet = false;

    Aws::Vector<Aws::String> m_subnetIds;
    bool m_subnetIdsHasBeenSet = false;

    Aws::Vector<Tag> m_tags;
    bool m_tagsHasBeenSet = false;

    Aws::String m_vpcId;
    bool m_vpcIdHasBeenSet = false;
  };

} // namespace Model
} // namespace WorkSpacesWeb
} // namespace Aws
