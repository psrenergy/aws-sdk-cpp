﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/network-firewall/NetworkFirewall_EXPORTS.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/client/AWSClient.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/network-firewall/NetworkFirewallServiceClientModel.h>
#include <aws/network-firewall/NetworkFirewallLegacyAsyncMacros.h>

namespace Aws
{
namespace NetworkFirewall
{
  /**
   * <p>This is the API Reference for Network Firewall. This guide is for developers
   * who need detailed information about the Network Firewall API actions, data
   * types, and errors. </p> <ul> <li> <p>The REST API requires you to handle
   * connection details, such as calculating signatures, handling request retries,
   * and error handling. For general information about using the Amazon Web Services
   * REST APIs, see <a
   * href="https://docs.aws.amazon.com/general/latest/gr/aws-apis.html">Amazon Web
   * Services APIs</a>. </p> <p>To access Network Firewall using the REST API
   * endpoint: <code>https://network-firewall.&lt;region&gt;.amazonaws.com </code>
   * </p> </li> <li> <p>Alternatively, you can use one of the Amazon Web Services
   * SDKs to access an API that's tailored to the programming language or platform
   * that you're using. For more information, see <a
   * href="http://aws.amazon.com/tools/#SDKs">Amazon Web Services SDKs</a>.</p> </li>
   * <li> <p>For descriptions of Network Firewall features, including and
   * step-by-step instructions on how to use them through the Network Firewall
   * console, see the <a
   * href="https://docs.aws.amazon.com/network-firewall/latest/developerguide/">Network
   * Firewall Developer Guide</a>.</p> </li> </ul> <p>Network Firewall is a stateful,
   * managed, network firewall and intrusion detection and prevention service for
   * Amazon Virtual Private Cloud (Amazon VPC). With Network Firewall, you can filter
   * traffic at the perimeter of your VPC. This includes filtering traffic going to
   * and coming from an internet gateway, NAT gateway, or over VPN or Direct Connect.
   * Network Firewall uses rules that are compatible with Suricata, a free, open
   * source network analysis and threat detection engine. Network Firewall supports
   * Suricata version 5.0.2. For information about Suricata, see the <a
   * href="https://suricata.io/">Suricata website</a>.</p> <p>You can use Network
   * Firewall to monitor and protect your VPC traffic in a number of ways. The
   * following are just a few examples: </p> <ul> <li> <p>Allow domains or IP
   * addresses for known Amazon Web Services service endpoints, such as Amazon S3,
   * and block all other forms of traffic.</p> </li> <li> <p>Use custom lists of
   * known bad domains to limit the types of domain names that your applications can
   * access.</p> </li> <li> <p>Perform deep packet inspection on traffic entering or
   * leaving your VPC.</p> </li> <li> <p>Use stateful protocol detection to filter
   * protocols like HTTPS, regardless of the port used.</p> </li> </ul> <p>To enable
   * Network Firewall for your VPCs, you perform steps in both Amazon VPC and in
   * Network Firewall. For information about using Amazon VPC, see <a
   * href="https://docs.aws.amazon.com/vpc/latest/userguide/">Amazon VPC User
   * Guide</a>.</p> <p>To start using Network Firewall, do the following: </p> <ol>
   * <li> <p>(Optional) If you don't already have a VPC that you want to protect,
   * create it in Amazon VPC. </p> </li> <li> <p>In Amazon VPC, in each Availability
   * Zone where you want to have a firewall endpoint, create a subnet for the sole
   * use of Network Firewall. </p> </li> <li> <p>In Network Firewall, create
   * stateless and stateful rule groups, to define the components of the network
   * traffic filtering behavior that you want your firewall to have. </p> </li> <li>
   * <p>In Network Firewall, create a firewall policy that uses your rule groups and
   * specifies additional default traffic filtering behavior. </p> </li> <li> <p>In
   * Network Firewall, create a firewall and specify your new firewall policy and VPC
   * subnets. Network Firewall creates a firewall endpoint in each subnet that you
   * specify, with the behavior that's defined in the firewall policy.</p> </li> <li>
   * <p>In Amazon VPC, use ingress routing enhancements to route traffic through the
   * new firewall endpoints.</p> </li> </ol>
   */
  class AWS_NETWORKFIREWALL_API NetworkFirewallClient : public Aws::Client::AWSJsonClient
  {
    public:
      typedef Aws::Client::AWSJsonClient BASECLASS;
      static const char* SERVICE_NAME;
      static const char* ALLOCATION_TAG;

       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        NetworkFirewallClient(const Aws::NetworkFirewall::NetworkFirewallClientConfiguration& clientConfiguration = Aws::NetworkFirewall::NetworkFirewallClientConfiguration(),
                              std::shared_ptr<NetworkFirewallEndpointProviderBase> endpointProvider = Aws::MakeShared<NetworkFirewallEndpointProvider>(ALLOCATION_TAG));

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        NetworkFirewallClient(const Aws::Auth::AWSCredentials& credentials,
                              std::shared_ptr<NetworkFirewallEndpointProviderBase> endpointProvider = Aws::MakeShared<NetworkFirewallEndpointProvider>(ALLOCATION_TAG),
                              const Aws::NetworkFirewall::NetworkFirewallClientConfiguration& clientConfiguration = Aws::NetworkFirewall::NetworkFirewallClientConfiguration());

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        NetworkFirewallClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                              std::shared_ptr<NetworkFirewallEndpointProviderBase> endpointProvider = Aws::MakeShared<NetworkFirewallEndpointProvider>(ALLOCATION_TAG),
                              const Aws::NetworkFirewall::NetworkFirewallClientConfiguration& clientConfiguration = Aws::NetworkFirewall::NetworkFirewallClientConfiguration());


        /* Legacy constructors due deprecation */
       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        NetworkFirewallClient(const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        NetworkFirewallClient(const Aws::Auth::AWSCredentials& credentials,
                              const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        NetworkFirewallClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                              const Aws::Client::ClientConfiguration& clientConfiguration);

        /* End of legacy constructors due deprecation */
        virtual ~NetworkFirewallClient();


        template<typename RequestT,
                 typename HandlerT,
                 typename HandlerContextT,
                 typename OperationFuncT>
        void SubmitAsync(OperationFuncT&& operationFunc,
                         const RequestT& request,
                         const HandlerT& handler,
                         const HandlerContextT& context)
        {
            Aws::Client::MakeAsyncOperation(std::forward<OperationFuncT>(operationFunc), this, request, handler, context, m_executor.get());
        }

        template<typename RequestT,
                 typename HandlerT,
                 typename HandlerContextT,
                 typename OperationFuncT>
        void SubmitAsync(OperationFuncT&& operationFunc,
                         RequestT& request,
                         const HandlerT& handler,
                         const HandlerContextT& context)
        {
            Aws::Client::MakeAsyncStreamingOperation(std::forward<OperationFuncT>(operationFunc), this, request, handler, context, m_executor.get());
        }

        template<typename RequestT,
                 typename OperationFuncT>
        auto SubmitCallable(OperationFuncT&& operationFunc,
                            const RequestT& request) -> std::future<decltype((this->*operationFunc)(request))>
        {
            return Aws::Client::MakeCallableOperation(ALLOCATION_TAG, operationFunc, this, request, m_executor.get());
        }

        template<typename RequestT,
                 typename OperationFuncT>
        auto SubmitCallable(OperationFuncT&& operationFunc,
                            RequestT& request) -> std::future<decltype((this->*operationFunc)(request))>
        {
            return Aws::Client::MakeCallableStreamingOperation(ALLOCATION_TAG, operationFunc, this, request, m_executor.get());
        }


        /**
         * <p>Associates a <a>FirewallPolicy</a> to a <a>Firewall</a>. </p> <p>A firewall
         * policy defines how to monitor and manage your VPC network traffic, using a
         * collection of inspection rule groups and other settings. Each firewall requires
         * one firewall policy association, and you can use the same firewall policy for
         * multiple firewalls. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/AssociateFirewallPolicy">AWS
         * API Reference</a></p>
         */
        virtual Model::AssociateFirewallPolicyOutcome AssociateFirewallPolicy(const Model::AssociateFirewallPolicyRequest& request) const;


        /**
         * <p>Associates the specified subnets in the Amazon VPC to the firewall. You can
         * specify one subnet for each of the Availability Zones that the VPC spans. </p>
         * <p>This request creates an Network Firewall firewall endpoint in each of the
         * subnets. To enable the firewall's protections, you must also modify the VPC's
         * route tables for each subnet's Availability Zone, to redirect the traffic that's
         * coming into and going out of the zone through the firewall endpoint.
         * </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/AssociateSubnets">AWS
         * API Reference</a></p>
         */
        virtual Model::AssociateSubnetsOutcome AssociateSubnets(const Model::AssociateSubnetsRequest& request) const;


        /**
         * <p>Creates an Network Firewall <a>Firewall</a> and accompanying
         * <a>FirewallStatus</a> for a VPC. </p> <p>The firewall defines the configuration
         * settings for an Network Firewall firewall. The settings that you can define at
         * creation include the firewall policy, the subnets in your VPC to use for the
         * firewall endpoints, and any tags that are attached to the firewall Amazon Web
         * Services resource. </p> <p>After you create a firewall, you can provide
         * additional settings, like the logging configuration. </p> <p>To update the
         * settings for a firewall, you use the operations that apply to the settings
         * themselves, for example <a>UpdateLoggingConfiguration</a>,
         * <a>AssociateSubnets</a>, and <a>UpdateFirewallDeleteProtection</a>. </p> <p>To
         * manage a firewall's tags, use the standard Amazon Web Services resource tagging
         * operations, <a>ListTagsForResource</a>, <a>TagResource</a>, and
         * <a>UntagResource</a>.</p> <p>To retrieve information about firewalls, use
         * <a>ListFirewalls</a> and <a>DescribeFirewall</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/CreateFirewall">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateFirewallOutcome CreateFirewall(const Model::CreateFirewallRequest& request) const;


        /**
         * <p>Creates the firewall policy for the firewall according to the specifications.
         * </p> <p>An Network Firewall firewall policy defines the behavior of a firewall,
         * in a collection of stateless and stateful rule groups and other settings. You
         * can use one firewall policy for multiple firewalls. </p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/CreateFirewallPolicy">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateFirewallPolicyOutcome CreateFirewallPolicy(const Model::CreateFirewallPolicyRequest& request) const;


        /**
         * <p>Creates the specified stateless or stateful rule group, which includes the
         * rules for network traffic inspection, a capacity setting, and tags. </p> <p>You
         * provide your rule group specification in your request using either
         * <code>RuleGroup</code> or <code>Rules</code>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/CreateRuleGroup">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateRuleGroupOutcome CreateRuleGroup(const Model::CreateRuleGroupRequest& request) const;


        /**
         * <p>Deletes the specified <a>Firewall</a> and its <a>FirewallStatus</a>. This
         * operation requires the firewall's <code>DeleteProtection</code> flag to be
         * <code>FALSE</code>. You can't revert this operation. </p> <p>You can check
         * whether a firewall is in use by reviewing the route tables for the Availability
         * Zones where you have firewall subnet mappings. Retrieve the subnet mappings by
         * calling <a>DescribeFirewall</a>. You define and update the route tables through
         * Amazon VPC. As needed, update the route tables for the zones to remove the
         * firewall endpoints. When the route tables no longer use the firewall endpoints,
         * you can remove the firewall safely.</p> <p>To delete a firewall, remove the
         * delete protection if you need to using <a>UpdateFirewallDeleteProtection</a>,
         * then delete the firewall by calling <a>DeleteFirewall</a>. </p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/DeleteFirewall">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteFirewallOutcome DeleteFirewall(const Model::DeleteFirewallRequest& request) const;


        /**
         * <p>Deletes the specified <a>FirewallPolicy</a>. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/DeleteFirewallPolicy">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteFirewallPolicyOutcome DeleteFirewallPolicy(const Model::DeleteFirewallPolicyRequest& request) const;


        /**
         * <p>Deletes a resource policy that you created in a <a>PutResourcePolicy</a>
         * request. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/DeleteResourcePolicy">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteResourcePolicyOutcome DeleteResourcePolicy(const Model::DeleteResourcePolicyRequest& request) const;


        /**
         * <p>Deletes the specified <a>RuleGroup</a>. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/DeleteRuleGroup">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteRuleGroupOutcome DeleteRuleGroup(const Model::DeleteRuleGroupRequest& request) const;


        /**
         * <p>Returns the data objects for the specified firewall. </p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/DescribeFirewall">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeFirewallOutcome DescribeFirewall(const Model::DescribeFirewallRequest& request) const;


        /**
         * <p>Returns the data objects for the specified firewall policy. </p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/DescribeFirewallPolicy">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeFirewallPolicyOutcome DescribeFirewallPolicy(const Model::DescribeFirewallPolicyRequest& request) const;


        /**
         * <p>Returns the logging configuration for the specified firewall. </p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/DescribeLoggingConfiguration">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeLoggingConfigurationOutcome DescribeLoggingConfiguration(const Model::DescribeLoggingConfigurationRequest& request) const;


        /**
         * <p>Retrieves a resource policy that you created in a <a>PutResourcePolicy</a>
         * request. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/DescribeResourcePolicy">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeResourcePolicyOutcome DescribeResourcePolicy(const Model::DescribeResourcePolicyRequest& request) const;


        /**
         * <p>Returns the data objects for the specified rule group. </p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/DescribeRuleGroup">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeRuleGroupOutcome DescribeRuleGroup(const Model::DescribeRuleGroupRequest& request) const;


        /**
         * <p>High-level information about a rule group, returned by operations like create
         * and describe. You can use the information provided in the metadata to retrieve
         * and manage a rule group. You can retrieve all objects for a rule group by
         * calling <a>DescribeRuleGroup</a>. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/DescribeRuleGroupMetadata">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeRuleGroupMetadataOutcome DescribeRuleGroupMetadata(const Model::DescribeRuleGroupMetadataRequest& request) const;


        /**
         * <p>Removes the specified subnet associations from the firewall. This removes the
         * firewall endpoints from the subnets and removes any network filtering
         * protections that the endpoints were providing. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/DisassociateSubnets">AWS
         * API Reference</a></p>
         */
        virtual Model::DisassociateSubnetsOutcome DisassociateSubnets(const Model::DisassociateSubnetsRequest& request) const;


        /**
         * <p>Retrieves the metadata for the firewall policies that you have defined.
         * Depending on your setting for max results and the number of firewall policies, a
         * single call might not return the full list. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/ListFirewallPolicies">AWS
         * API Reference</a></p>
         */
        virtual Model::ListFirewallPoliciesOutcome ListFirewallPolicies(const Model::ListFirewallPoliciesRequest& request) const;


        /**
         * <p>Retrieves the metadata for the firewalls that you have defined. If you
         * provide VPC identifiers in your request, this returns only the firewalls for
         * those VPCs.</p> <p>Depending on your setting for max results and the number of
         * firewalls, a single call might not return the full list. </p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/ListFirewalls">AWS
         * API Reference</a></p>
         */
        virtual Model::ListFirewallsOutcome ListFirewalls(const Model::ListFirewallsRequest& request) const;


        /**
         * <p>Retrieves the metadata for the rule groups that you have defined. Depending
         * on your setting for max results and the number of rule groups, a single call
         * might not return the full list. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/ListRuleGroups">AWS
         * API Reference</a></p>
         */
        virtual Model::ListRuleGroupsOutcome ListRuleGroups(const Model::ListRuleGroupsRequest& request) const;


        /**
         * <p>Retrieves the tags associated with the specified resource. Tags are key:value
         * pairs that you can use to categorize and manage your resources, for purposes
         * like billing. For example, you might set the tag key to "customer" and the value
         * to the customer name or ID. You can specify one or more tags to add to each
         * Amazon Web Services resource, up to 50 tags for a resource.</p> <p>You can tag
         * the Amazon Web Services resources that you manage through Network Firewall:
         * firewalls, firewall policies, and rule groups. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/ListTagsForResource">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTagsForResourceOutcome ListTagsForResource(const Model::ListTagsForResourceRequest& request) const;


        /**
         * <p>Creates or updates an IAM policy for your rule group or firewall policy. Use
         * this to share rule groups and firewall policies between accounts. This operation
         * works in conjunction with the Amazon Web Services Resource Access Manager (RAM)
         * service to manage resource sharing for Network Firewall. </p> <p>Use this
         * operation to create or update a resource policy for your rule group or firewall
         * policy. In the policy, you specify the accounts that you want to share the
         * resource with and the operations that you want the accounts to be able to
         * perform. </p> <p>When you add an account in the resource policy, you then run
         * the following Resource Access Manager (RAM) operations to access and accept the
         * shared rule group or firewall policy. </p> <ul> <li> <p> <a
         * href="https://docs.aws.amazon.com/ram/latest/APIReference/API_GetResourceShareInvitations.html">GetResourceShareInvitations</a>
         * - Returns the Amazon Resource Names (ARNs) of the resource share invitations.
         * </p> </li> <li> <p> <a
         * href="https://docs.aws.amazon.com/ram/latest/APIReference/API_AcceptResourceShareInvitation.html">AcceptResourceShareInvitation</a>
         * - Accepts the share invitation for a specified resource share. </p> </li> </ul>
         * <p>For additional information about resource sharing using RAM, see <a
         * href="https://docs.aws.amazon.com/ram/latest/userguide/what-is.html">Resource
         * Access Manager User Guide</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/PutResourcePolicy">AWS
         * API Reference</a></p>
         */
        virtual Model::PutResourcePolicyOutcome PutResourcePolicy(const Model::PutResourcePolicyRequest& request) const;


        /**
         * <p>Adds the specified tags to the specified resource. Tags are key:value pairs
         * that you can use to categorize and manage your resources, for purposes like
         * billing. For example, you might set the tag key to "customer" and the value to
         * the customer name or ID. You can specify one or more tags to add to each Amazon
         * Web Services resource, up to 50 tags for a resource.</p> <p>You can tag the
         * Amazon Web Services resources that you manage through Network Firewall:
         * firewalls, firewall policies, and rule groups. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/TagResource">AWS
         * API Reference</a></p>
         */
        virtual Model::TagResourceOutcome TagResource(const Model::TagResourceRequest& request) const;


        /**
         * <p>Removes the tags with the specified keys from the specified resource. Tags
         * are key:value pairs that you can use to categorize and manage your resources,
         * for purposes like billing. For example, you might set the tag key to "customer"
         * and the value to the customer name or ID. You can specify one or more tags to
         * add to each Amazon Web Services resource, up to 50 tags for a resource.</p>
         * <p>You can manage tags for the Amazon Web Services resources that you manage
         * through Network Firewall: firewalls, firewall policies, and rule groups.
         * </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/UntagResource">AWS
         * API Reference</a></p>
         */
        virtual Model::UntagResourceOutcome UntagResource(const Model::UntagResourceRequest& request) const;


        /**
         * <p>Modifies the flag, <code>DeleteProtection</code>, which indicates whether it
         * is possible to delete the firewall. If the flag is set to <code>TRUE</code>, the
         * firewall is protected against deletion. This setting helps protect against
         * accidentally deleting a firewall that's in use. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/UpdateFirewallDeleteProtection">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateFirewallDeleteProtectionOutcome UpdateFirewallDeleteProtection(const Model::UpdateFirewallDeleteProtectionRequest& request) const;


        /**
         * <p>Modifies the description for the specified firewall. Use the description to
         * help you identify the firewall when you're working with it. </p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/UpdateFirewallDescription">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateFirewallDescriptionOutcome UpdateFirewallDescription(const Model::UpdateFirewallDescriptionRequest& request) const;


        /**
         * <p>A complex type that contains settings for encryption of your firewall
         * resources.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/UpdateFirewallEncryptionConfiguration">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateFirewallEncryptionConfigurationOutcome UpdateFirewallEncryptionConfiguration(const Model::UpdateFirewallEncryptionConfigurationRequest& request) const;


        /**
         * <p>Updates the properties of the specified firewall policy.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/UpdateFirewallPolicy">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateFirewallPolicyOutcome UpdateFirewallPolicy(const Model::UpdateFirewallPolicyRequest& request) const;


        /**
         * <p>Modifies the flag, <code>ChangeProtection</code>, which indicates whether it
         * is possible to change the firewall. If the flag is set to <code>TRUE</code>, the
         * firewall is protected from changes. This setting helps protect against
         * accidentally changing a firewall that's in use.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/UpdateFirewallPolicyChangeProtection">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateFirewallPolicyChangeProtectionOutcome UpdateFirewallPolicyChangeProtection(const Model::UpdateFirewallPolicyChangeProtectionRequest& request) const;


        /**
         * <p>Sets the logging configuration for the specified firewall. </p> <p>To change
         * the logging configuration, retrieve the <a>LoggingConfiguration</a> by calling
         * <a>DescribeLoggingConfiguration</a>, then change it and provide the modified
         * object to this update call. You must change the logging configuration one
         * <a>LogDestinationConfig</a> at a time inside the retrieved
         * <a>LoggingConfiguration</a> object. </p> <p>You can perform only one of the
         * following actions in any call to <code>UpdateLoggingConfiguration</code>: </p>
         * <ul> <li> <p>Create a new log destination object by adding a single
         * <code>LogDestinationConfig</code> array element to
         * <code>LogDestinationConfigs</code>.</p> </li> <li> <p>Delete a log destination
         * object by removing a single <code>LogDestinationConfig</code> array element from
         * <code>LogDestinationConfigs</code>.</p> </li> <li> <p>Change the
         * <code>LogDestination</code> setting in a single
         * <code>LogDestinationConfig</code> array element.</p> </li> </ul> <p>You can't
         * change the <code>LogDestinationType</code> or <code>LogType</code> in a
         * <code>LogDestinationConfig</code>. To change these settings, delete the existing
         * <code>LogDestinationConfig</code> object and create a new one, using two
         * separate calls to this update operation.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/UpdateLoggingConfiguration">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateLoggingConfigurationOutcome UpdateLoggingConfiguration(const Model::UpdateLoggingConfigurationRequest& request) const;


        /**
         * <p>Updates the rule settings for the specified rule group. You use a rule group
         * by reference in one or more firewall policies. When you modify a rule group, you
         * modify all firewall policies that use the rule group. </p> <p>To update a rule
         * group, first call <a>DescribeRuleGroup</a> to retrieve the current
         * <a>RuleGroup</a> object, update the object as needed, and then provide the
         * updated object to this call. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/UpdateRuleGroup">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateRuleGroupOutcome UpdateRuleGroup(const Model::UpdateRuleGroupRequest& request) const;


        /**
         * <p/><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/network-firewall-2020-11-12/UpdateSubnetChangeProtection">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateSubnetChangeProtectionOutcome UpdateSubnetChangeProtection(const Model::UpdateSubnetChangeProtectionRequest& request) const;



      void OverrideEndpoint(const Aws::String& endpoint);
      std::shared_ptr<NetworkFirewallEndpointProviderBase>& accessEndpointProvider();
    private:
      void init(const NetworkFirewallClientConfiguration& clientConfiguration);

      NetworkFirewallClientConfiguration m_clientConfiguration;
      std::shared_ptr<Aws::Utils::Threading::Executor> m_executor;
      std::shared_ptr<NetworkFirewallEndpointProviderBase> m_endpointProvider;
  };

} // namespace NetworkFirewall
} // namespace Aws
