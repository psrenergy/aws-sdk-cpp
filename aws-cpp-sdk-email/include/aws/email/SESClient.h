﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/email/SES_EXPORTS.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/AmazonSerializableWebServiceRequest.h>
#include <aws/core/client/AWSClient.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/utils/xml/XmlSerializer.h>
#include <aws/email/SESServiceClientModel.h>
#include <aws/email/SESLegacyAsyncMacros.h>

namespace Aws
{
namespace SES
{
  /**
   * <fullname>Amazon Simple Email Service</fullname> <p> This document contains
   * reference information for the <a href="https://aws.amazon.com/ses/">Amazon
   * Simple Email Service</a> (Amazon SES) API, version 2010-12-01. This document is
   * best used in conjunction with the <a
   * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/Welcome.html">Amazon
   * SES Developer Guide</a>. </p>  <p> For a list of Amazon SES endpoints to
   * use in service requests, see <a
   * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/regions.html">Regions
   * and Amazon SES</a> in the <a
   * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/Welcome.html">Amazon
   * SES Developer Guide</a>.</p> 
   */
  class AWS_SES_API SESClient : public Aws::Client::AWSXMLClient
  {
    public:
      typedef Aws::Client::AWSXMLClient BASECLASS;
      static const char* SERVICE_NAME;
      static const char* ALLOCATION_TAG;

       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        SESClient(const Aws::SES::SESClientConfiguration& clientConfiguration = Aws::SES::SESClientConfiguration(),
                  std::shared_ptr<SESEndpointProviderBase> endpointProvider = Aws::MakeShared<SESEndpointProvider>(ALLOCATION_TAG));

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        SESClient(const Aws::Auth::AWSCredentials& credentials,
                  std::shared_ptr<SESEndpointProviderBase> endpointProvider = Aws::MakeShared<SESEndpointProvider>(ALLOCATION_TAG),
                  const Aws::SES::SESClientConfiguration& clientConfiguration = Aws::SES::SESClientConfiguration());

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        SESClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                  std::shared_ptr<SESEndpointProviderBase> endpointProvider = Aws::MakeShared<SESEndpointProvider>(ALLOCATION_TAG),
                  const Aws::SES::SESClientConfiguration& clientConfiguration = Aws::SES::SESClientConfiguration());


        /* Legacy constructors due deprecation */
       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        SESClient(const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        SESClient(const Aws::Auth::AWSCredentials& credentials,
                  const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        SESClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                  const Aws::Client::ClientConfiguration& clientConfiguration);

        /* End of legacy constructors due deprecation */
        virtual ~SESClient();


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
        * Converts any request object to a presigned URL with the GET method, using region for the signer and a timeout of 15 minutes.
        */
        Aws::String ConvertRequestToPresignedUrl(const Aws::AmazonSerializableWebServiceRequest& requestToConvert, const char* region) const;


        /**
         * <p>Creates a receipt rule set by cloning an existing one. All receipt rules and
         * configurations are copied to the new receipt rule set and are completely
         * independent of the source rule set.</p> <p>For information about setting up rule
         * sets, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-receipt-rule-set.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/CloneReceiptRuleSet">AWS
         * API Reference</a></p>
         */
        virtual Model::CloneReceiptRuleSetOutcome CloneReceiptRuleSet(const Model::CloneReceiptRuleSetRequest& request) const;


        /**
         * <p>Creates a configuration set.</p> <p>Configuration sets enable you to publish
         * email sending events. For information about using configuration sets, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/monitor-sending-activity.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/CreateConfigurationSet">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateConfigurationSetOutcome CreateConfigurationSet(const Model::CreateConfigurationSetRequest& request) const;


        /**
         * <p>Creates a configuration set event destination.</p>  <p>When you create
         * or update an event destination, you must provide one, and only one, destination.
         * The destination can be CloudWatch, Amazon Kinesis Firehose, or Amazon Simple
         * Notification Service (Amazon SNS).</p>  <p>An event destination is the
         * AWS service to which Amazon SES publishes the email sending events associated
         * with a configuration set. For information about using configuration sets, see
         * the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/monitor-sending-activity.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/CreateConfigurationSetEventDestination">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateConfigurationSetEventDestinationOutcome CreateConfigurationSetEventDestination(const Model::CreateConfigurationSetEventDestinationRequest& request) const;


        /**
         * <p>Creates an association between a configuration set and a custom domain for
         * open and click event tracking. </p> <p>By default, images and links used for
         * tracking open and click events are hosted on domains operated by Amazon SES. You
         * can configure a subdomain of your own to handle these events. For information
         * about using custom domains, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/configure-custom-open-click-domains.html">Amazon
         * SES Developer Guide</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/CreateConfigurationSetTrackingOptions">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateConfigurationSetTrackingOptionsOutcome CreateConfigurationSetTrackingOptions(const Model::CreateConfigurationSetTrackingOptionsRequest& request) const;


        /**
         * <p>Creates a new custom verification email template.</p> <p>For more information
         * about custom verification email templates, see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/custom-verification-emails.html">Using
         * Custom Verification Email Templates</a> in the <i>Amazon SES Developer
         * Guide</i>.</p> <p>You can execute this operation no more than once per
         * second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/CreateCustomVerificationEmailTemplate">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateCustomVerificationEmailTemplateOutcome CreateCustomVerificationEmailTemplate(const Model::CreateCustomVerificationEmailTemplateRequest& request) const;


        /**
         * <p>Creates a new IP address filter.</p> <p>For information about setting up IP
         * address filters, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-ip-filters.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/CreateReceiptFilter">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateReceiptFilterOutcome CreateReceiptFilter(const Model::CreateReceiptFilterRequest& request) const;


        /**
         * <p>Creates a receipt rule.</p> <p>For information about setting up receipt
         * rules, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-receipt-rules.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/CreateReceiptRule">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateReceiptRuleOutcome CreateReceiptRule(const Model::CreateReceiptRuleRequest& request) const;


        /**
         * <p>Creates an empty receipt rule set.</p> <p>For information about setting up
         * receipt rule sets, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-receipt-rule-set.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/CreateReceiptRuleSet">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateReceiptRuleSetOutcome CreateReceiptRuleSet(const Model::CreateReceiptRuleSetRequest& request) const;


        /**
         * <p>Creates an email template. Email templates enable you to send personalized
         * email to one or more destinations in a single API operation. For more
         * information, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/send-personalized-email-api.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/CreateTemplate">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateTemplateOutcome CreateTemplate(const Model::CreateTemplateRequest& request) const;


        /**
         * <p>Deletes a configuration set. Configuration sets enable you to publish email
         * sending events. For information about using configuration sets, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/monitor-sending-activity.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/DeleteConfigurationSet">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteConfigurationSetOutcome DeleteConfigurationSet(const Model::DeleteConfigurationSetRequest& request) const;


        /**
         * <p>Deletes a configuration set event destination. Configuration set event
         * destinations are associated with configuration sets, which enable you to publish
         * email sending events. For information about using configuration sets, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/monitor-sending-activity.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/DeleteConfigurationSetEventDestination">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteConfigurationSetEventDestinationOutcome DeleteConfigurationSetEventDestination(const Model::DeleteConfigurationSetEventDestinationRequest& request) const;


        /**
         * <p>Deletes an association between a configuration set and a custom domain for
         * open and click event tracking.</p> <p>By default, images and links used for
         * tracking open and click events are hosted on domains operated by Amazon SES. You
         * can configure a subdomain of your own to handle these events. For information
         * about using custom domains, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/configure-custom-open-click-domains.html">Amazon
         * SES Developer Guide</a>.</p>  <p>Deleting this kind of association will
         * result in emails sent using the specified configuration set to capture open and
         * click events using the standard, Amazon SES-operated domains.</p>
         * <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/DeleteConfigurationSetTrackingOptions">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteConfigurationSetTrackingOptionsOutcome DeleteConfigurationSetTrackingOptions(const Model::DeleteConfigurationSetTrackingOptionsRequest& request) const;


        /**
         * <p>Deletes an existing custom verification email template. </p> <p>For more
         * information about custom verification email templates, see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/custom-verification-emails.html">Using
         * Custom Verification Email Templates</a> in the <i>Amazon SES Developer
         * Guide</i>.</p> <p>You can execute this operation no more than once per
         * second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/DeleteCustomVerificationEmailTemplate">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteCustomVerificationEmailTemplateOutcome DeleteCustomVerificationEmailTemplate(const Model::DeleteCustomVerificationEmailTemplateRequest& request) const;


        /**
         * <p>Deletes the specified identity (an email address or a domain) from the list
         * of verified identities.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/DeleteIdentity">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteIdentityOutcome DeleteIdentity(const Model::DeleteIdentityRequest& request) const;


        /**
         * <p>Deletes the specified sending authorization policy for the given identity (an
         * email address or a domain). This API returns successfully even if a policy with
         * the specified name does not exist.</p>  <p>This API is for the identity
         * owner only. If you have not verified the identity, this API will return an
         * error.</p>  <p>Sending authorization is a feature that enables an
         * identity owner to authorize other senders to use its identities. For information
         * about using sending authorization, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/sending-authorization.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/DeleteIdentityPolicy">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteIdentityPolicyOutcome DeleteIdentityPolicy(const Model::DeleteIdentityPolicyRequest& request) const;


        /**
         * <p>Deletes the specified IP address filter.</p> <p>For information about
         * managing IP address filters, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-managing-ip-filters.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/DeleteReceiptFilter">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteReceiptFilterOutcome DeleteReceiptFilter(const Model::DeleteReceiptFilterRequest& request) const;


        /**
         * <p>Deletes the specified receipt rule.</p> <p>For information about managing
         * receipt rules, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-managing-receipt-rules.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/DeleteReceiptRule">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteReceiptRuleOutcome DeleteReceiptRule(const Model::DeleteReceiptRuleRequest& request) const;


        /**
         * <p>Deletes the specified receipt rule set and all of the receipt rules it
         * contains.</p>  <p>The currently active rule set cannot be deleted.</p>
         *  <p>For information about managing receipt rule sets, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-managing-receipt-rule-sets.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/DeleteReceiptRuleSet">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteReceiptRuleSetOutcome DeleteReceiptRuleSet(const Model::DeleteReceiptRuleSetRequest& request) const;


        /**
         * <p>Deletes an email template.</p> <p>You can execute this operation no more than
         * once per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/DeleteTemplate">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteTemplateOutcome DeleteTemplate(const Model::DeleteTemplateRequest& request) const;


        /**
         * <p>Deprecated. Use the <code>DeleteIdentity</code> operation to delete email
         * addresses and domains.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/DeleteVerifiedEmailAddress">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteVerifiedEmailAddressOutcome DeleteVerifiedEmailAddress(const Model::DeleteVerifiedEmailAddressRequest& request) const;


        /**
         * <p>Returns the metadata and receipt rules for the receipt rule set that is
         * currently active.</p> <p>For information about setting up receipt rule sets, see
         * the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-receipt-rule-set.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/DescribeActiveReceiptRuleSet">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeActiveReceiptRuleSetOutcome DescribeActiveReceiptRuleSet(const Model::DescribeActiveReceiptRuleSetRequest& request) const;


        /**
         * <p>Returns the details of the specified configuration set. For information about
         * using configuration sets, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/monitor-sending-activity.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/DescribeConfigurationSet">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeConfigurationSetOutcome DescribeConfigurationSet(const Model::DescribeConfigurationSetRequest& request) const;


        /**
         * <p>Returns the details of the specified receipt rule.</p> <p>For information
         * about setting up receipt rules, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-receipt-rules.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/DescribeReceiptRule">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeReceiptRuleOutcome DescribeReceiptRule(const Model::DescribeReceiptRuleRequest& request) const;


        /**
         * <p>Returns the details of the specified receipt rule set.</p> <p>For information
         * about managing receipt rule sets, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-managing-receipt-rule-sets.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/DescribeReceiptRuleSet">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeReceiptRuleSetOutcome DescribeReceiptRuleSet(const Model::DescribeReceiptRuleSetRequest& request) const;


        /**
         * <p>Returns the email sending status of the Amazon SES account for the current
         * region.</p> <p>You can execute this operation no more than once per
         * second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/GetAccountSendingEnabled">AWS
         * API Reference</a></p>
         */
        virtual Model::GetAccountSendingEnabledOutcome GetAccountSendingEnabled(const Model::GetAccountSendingEnabledRequest& request) const;


        /**
         * <p>Returns the custom email verification template for the template name you
         * specify.</p> <p>For more information about custom verification email templates,
         * see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/custom-verification-emails.html">Using
         * Custom Verification Email Templates</a> in the <i>Amazon SES Developer
         * Guide</i>.</p> <p>You can execute this operation no more than once per
         * second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/GetCustomVerificationEmailTemplate">AWS
         * API Reference</a></p>
         */
        virtual Model::GetCustomVerificationEmailTemplateOutcome GetCustomVerificationEmailTemplate(const Model::GetCustomVerificationEmailTemplateRequest& request) const;


        /**
         * <p>Returns the current status of Easy DKIM signing for an entity. For domain
         * name identities, this operation also returns the DKIM tokens that are required
         * for Easy DKIM signing, and whether Amazon SES has successfully verified that
         * these tokens have been published.</p> <p>This operation takes a list of
         * identities as input and returns the following information for each:</p> <ul>
         * <li> <p>Whether Easy DKIM signing is enabled or disabled.</p> </li> <li> <p>A
         * set of DKIM tokens that represent the identity. If the identity is an email
         * address, the tokens represent the domain of that address.</p> </li> <li>
         * <p>Whether Amazon SES has successfully verified the DKIM tokens published in the
         * domain's DNS. This information is only returned for domain name identities, not
         * for email addresses.</p> </li> </ul> <p>This operation is throttled at one
         * request per second and can only get DKIM attributes for up to 100 identities at
         * a time.</p> <p>For more information about creating DNS records using DKIM
         * tokens, go to the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/easy-dkim-dns-records.html">Amazon
         * SES Developer Guide</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/GetIdentityDkimAttributes">AWS
         * API Reference</a></p>
         */
        virtual Model::GetIdentityDkimAttributesOutcome GetIdentityDkimAttributes(const Model::GetIdentityDkimAttributesRequest& request) const;


        /**
         * <p>Returns the custom MAIL FROM attributes for a list of identities (email
         * addresses : domains).</p> <p>This operation is throttled at one request per
         * second and can only get custom MAIL FROM attributes for up to 100 identities at
         * a time.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/GetIdentityMailFromDomainAttributes">AWS
         * API Reference</a></p>
         */
        virtual Model::GetIdentityMailFromDomainAttributesOutcome GetIdentityMailFromDomainAttributes(const Model::GetIdentityMailFromDomainAttributesRequest& request) const;


        /**
         * <p>Given a list of verified identities (email addresses and/or domains), returns
         * a structure describing identity notification attributes.</p> <p>This operation
         * is throttled at one request per second and can only get notification attributes
         * for up to 100 identities at a time.</p> <p>For more information about using
         * notifications with Amazon SES, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/notifications.html">Amazon
         * SES Developer Guide</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/GetIdentityNotificationAttributes">AWS
         * API Reference</a></p>
         */
        virtual Model::GetIdentityNotificationAttributesOutcome GetIdentityNotificationAttributes(const Model::GetIdentityNotificationAttributesRequest& request) const;


        /**
         * <p>Returns the requested sending authorization policies for the given identity
         * (an email address or a domain). The policies are returned as a map of policy
         * names to policy contents. You can retrieve a maximum of 20 policies at a
         * time.</p>  <p>This API is for the identity owner only. If you have not
         * verified the identity, this API will return an error.</p>  <p>Sending
         * authorization is a feature that enables an identity owner to authorize other
         * senders to use its identities. For information about using sending
         * authorization, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/sending-authorization.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/GetIdentityPolicies">AWS
         * API Reference</a></p>
         */
        virtual Model::GetIdentityPoliciesOutcome GetIdentityPolicies(const Model::GetIdentityPoliciesRequest& request) const;


        /**
         * <p>Given a list of identities (email addresses and/or domains), returns the
         * verification status and (for domain identities) the verification token for each
         * identity.</p> <p>The verification status of an email address is "Pending" until
         * the email address owner clicks the link within the verification email that
         * Amazon SES sent to that address. If the email address owner clicks the link
         * within 24 hours, the verification status of the email address changes to
         * "Success". If the link is not clicked within 24 hours, the verification status
         * changes to "Failed." In that case, if you still want to verify the email
         * address, you must restart the verification process from the beginning.</p>
         * <p>For domain identities, the domain's verification status is "Pending" as
         * Amazon SES searches for the required TXT record in the DNS settings of the
         * domain. When Amazon SES detects the record, the domain's verification status
         * changes to "Success". If Amazon SES is unable to detect the record within 72
         * hours, the domain's verification status changes to "Failed." In that case, if
         * you still want to verify the domain, you must restart the verification process
         * from the beginning.</p> <p>This operation is throttled at one request per second
         * and can only get verification attributes for up to 100 identities at a
         * time.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/GetIdentityVerificationAttributes">AWS
         * API Reference</a></p>
         */
        virtual Model::GetIdentityVerificationAttributesOutcome GetIdentityVerificationAttributes(const Model::GetIdentityVerificationAttributesRequest& request) const;


        /**
         * <p>Provides the sending limits for the Amazon SES account. </p> <p>You can
         * execute this operation no more than once per second.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/GetSendQuota">AWS
         * API Reference</a></p>
         */
        virtual Model::GetSendQuotaOutcome GetSendQuota(const Model::GetSendQuotaRequest& request) const;


        /**
         * <p>Provides sending statistics for the current AWS Region. The result is a list
         * of data points, representing the last two weeks of sending activity. Each data
         * point in the list contains statistics for a 15-minute period of time.</p> <p>You
         * can execute this operation no more than once per second.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/GetSendStatistics">AWS
         * API Reference</a></p>
         */
        virtual Model::GetSendStatisticsOutcome GetSendStatistics(const Model::GetSendStatisticsRequest& request) const;


        /**
         * <p>Displays the template object (which includes the Subject line, HTML part and
         * text part) for the template you specify.</p> <p>You can execute this operation
         * no more than once per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/GetTemplate">AWS
         * API Reference</a></p>
         */
        virtual Model::GetTemplateOutcome GetTemplate(const Model::GetTemplateRequest& request) const;


        /**
         * <p>Provides a list of the configuration sets associated with your Amazon SES
         * account in the current AWS Region. For information about using configuration
         * sets, see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/monitor-sending-activity.html">Monitoring
         * Your Amazon SES Sending Activity</a> in the <i>Amazon SES Developer Guide.</i>
         * </p> <p>You can execute this operation no more than once per second. This
         * operation will return up to 1,000 configuration sets each time it is run. If
         * your Amazon SES account has more than 1,000 configuration sets, this operation
         * will also return a NextToken element. You can then execute the
         * <code>ListConfigurationSets</code> operation again, passing the
         * <code>NextToken</code> parameter and the value of the NextToken element to
         * retrieve additional results.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/ListConfigurationSets">AWS
         * API Reference</a></p>
         */
        virtual Model::ListConfigurationSetsOutcome ListConfigurationSets(const Model::ListConfigurationSetsRequest& request) const;


        /**
         * <p>Lists the existing custom verification email templates for your account in
         * the current AWS Region.</p> <p>For more information about custom verification
         * email templates, see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/custom-verification-emails.html">Using
         * Custom Verification Email Templates</a> in the <i>Amazon SES Developer
         * Guide</i>.</p> <p>You can execute this operation no more than once per
         * second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/ListCustomVerificationEmailTemplates">AWS
         * API Reference</a></p>
         */
        virtual Model::ListCustomVerificationEmailTemplatesOutcome ListCustomVerificationEmailTemplates(const Model::ListCustomVerificationEmailTemplatesRequest& request) const;


        /**
         * <p>Returns a list containing all of the identities (email addresses and domains)
         * for your AWS account in the current AWS Region, regardless of verification
         * status.</p> <p>You can execute this operation no more than once per
         * second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/ListIdentities">AWS
         * API Reference</a></p>
         */
        virtual Model::ListIdentitiesOutcome ListIdentities(const Model::ListIdentitiesRequest& request) const;


        /**
         * <p>Returns a list of sending authorization policies that are attached to the
         * given identity (an email address or a domain). This API returns only a list. If
         * you want the actual policy content, you can use
         * <code>GetIdentityPolicies</code>.</p>  <p>This API is for the identity
         * owner only. If you have not verified the identity, this API will return an
         * error.</p>  <p>Sending authorization is a feature that enables an
         * identity owner to authorize other senders to use its identities. For information
         * about using sending authorization, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/sending-authorization.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/ListIdentityPolicies">AWS
         * API Reference</a></p>
         */
        virtual Model::ListIdentityPoliciesOutcome ListIdentityPolicies(const Model::ListIdentityPoliciesRequest& request) const;


        /**
         * <p>Lists the IP address filters associated with your AWS account in the current
         * AWS Region.</p> <p>For information about managing IP address filters, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-managing-ip-filters.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/ListReceiptFilters">AWS
         * API Reference</a></p>
         */
        virtual Model::ListReceiptFiltersOutcome ListReceiptFilters(const Model::ListReceiptFiltersRequest& request) const;


        /**
         * <p>Lists the receipt rule sets that exist under your AWS account in the current
         * AWS Region. If there are additional receipt rule sets to be retrieved, you will
         * receive a <code>NextToken</code> that you can provide to the next call to
         * <code>ListReceiptRuleSets</code> to retrieve the additional entries.</p> <p>For
         * information about managing receipt rule sets, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-managing-receipt-rule-sets.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/ListReceiptRuleSets">AWS
         * API Reference</a></p>
         */
        virtual Model::ListReceiptRuleSetsOutcome ListReceiptRuleSets(const Model::ListReceiptRuleSetsRequest& request) const;


        /**
         * <p>Lists the email templates present in your Amazon SES account in the current
         * AWS Region.</p> <p>You can execute this operation no more than once per
         * second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/ListTemplates">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTemplatesOutcome ListTemplates(const Model::ListTemplatesRequest& request) const;


        /**
         * <p>Deprecated. Use the <code>ListIdentities</code> operation to list the email
         * addresses and domains associated with your account.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/ListVerifiedEmailAddresses">AWS
         * API Reference</a></p>
         */
        virtual Model::ListVerifiedEmailAddressesOutcome ListVerifiedEmailAddresses(const Model::ListVerifiedEmailAddressesRequest& request) const;


        /**
         * <p>Adds or updates the delivery options for a configuration set.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/PutConfigurationSetDeliveryOptions">AWS
         * API Reference</a></p>
         */
        virtual Model::PutConfigurationSetDeliveryOptionsOutcome PutConfigurationSetDeliveryOptions(const Model::PutConfigurationSetDeliveryOptionsRequest& request) const;


        /**
         * <p>Adds or updates a sending authorization policy for the specified identity (an
         * email address or a domain).</p>  <p>This API is for the identity owner
         * only. If you have not verified the identity, this API will return an error.</p>
         *  <p>Sending authorization is a feature that enables an identity owner to
         * authorize other senders to use its identities. For information about using
         * sending authorization, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/sending-authorization.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/PutIdentityPolicy">AWS
         * API Reference</a></p>
         */
        virtual Model::PutIdentityPolicyOutcome PutIdentityPolicy(const Model::PutIdentityPolicyRequest& request) const;


        /**
         * <p>Reorders the receipt rules within a receipt rule set.</p>  <p>All of
         * the rules in the rule set must be represented in this request. That is, this API
         * will return an error if the reorder request doesn't explicitly position all of
         * the rules.</p>  <p>For information about managing receipt rule sets, see
         * the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-managing-receipt-rule-sets.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/ReorderReceiptRuleSet">AWS
         * API Reference</a></p>
         */
        virtual Model::ReorderReceiptRuleSetOutcome ReorderReceiptRuleSet(const Model::ReorderReceiptRuleSetRequest& request) const;


        /**
         * <p>Generates and sends a bounce message to the sender of an email you received
         * through Amazon SES. You can only use this API on an email up to 24 hours after
         * you receive it.</p>  <p>You cannot use this API to send generic bounces
         * for mail that was not received by Amazon SES.</p>  <p>For information
         * about receiving email through Amazon SES, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/SendBounce">AWS
         * API Reference</a></p>
         */
        virtual Model::SendBounceOutcome SendBounce(const Model::SendBounceRequest& request) const;


        /**
         * <p>Composes an email message to multiple destinations. The message body is
         * created using an email template.</p> <p>In order to send email using the
         * <code>SendBulkTemplatedEmail</code> operation, your call to the API must meet
         * the following requirements:</p> <ul> <li> <p>The call must refer to an existing
         * email template. You can create email templates using the <a>CreateTemplate</a>
         * operation.</p> </li> <li> <p>The message must be sent from a verified email
         * address or domain.</p> </li> <li> <p>If your account is still in the Amazon SES
         * sandbox, you may only send to verified addresses or domains, or to email
         * addresses associated with the Amazon SES Mailbox Simulator. For more
         * information, see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/verify-addresses-and-domains.html">Verifying
         * Email Addresses and Domains</a> in the <i>Amazon SES Developer Guide.</i> </p>
         * </li> <li> <p>The maximum message size is 10 MB.</p> </li> <li> <p>Each
         * <code>Destination</code> parameter must include at least one recipient email
         * address. The recipient address can be a To: address, a CC: address, or a BCC:
         * address. If a recipient email address is invalid (that is, it is not in the
         * format <i>UserName@[SubDomain.]Domain.TopLevelDomain</i>), the entire message
         * will be rejected, even if the message contains other recipients that are
         * valid.</p> </li> <li> <p>The message may not include more than 50 recipients,
         * across the To:, CC: and BCC: fields. If you need to send an email message to a
         * larger audience, you can divide your recipient list into groups of 50 or fewer,
         * and then call the <code>SendBulkTemplatedEmail</code> operation several times to
         * send the message to each group.</p> </li> <li> <p>The number of destinations you
         * can contact in a single call to the API may be limited by your account's maximum
         * sending rate.</p> </li> </ul><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/SendBulkTemplatedEmail">AWS
         * API Reference</a></p>
         */
        virtual Model::SendBulkTemplatedEmailOutcome SendBulkTemplatedEmail(const Model::SendBulkTemplatedEmailRequest& request) const;


        /**
         * <p>Adds an email address to the list of identities for your Amazon SES account
         * in the current AWS Region and attempts to verify it. As a result of executing
         * this operation, a customized verification email is sent to the specified
         * address.</p> <p>To use this operation, you must first create a custom
         * verification email template. For more information about creating and using
         * custom verification email templates, see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/custom-verification-emails.html">Using
         * Custom Verification Email Templates</a> in the <i>Amazon SES Developer
         * Guide</i>.</p> <p>You can execute this operation no more than once per
         * second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/SendCustomVerificationEmail">AWS
         * API Reference</a></p>
         */
        virtual Model::SendCustomVerificationEmailOutcome SendCustomVerificationEmail(const Model::SendCustomVerificationEmailRequest& request) const;


        /**
         * <p>Composes an email message and immediately queues it for sending. In order to
         * send email using the <code>SendEmail</code> operation, your message must meet
         * the following requirements:</p> <ul> <li> <p>The message must be sent from a
         * verified email address or domain. If you attempt to send email using a
         * non-verified address or domain, the operation will result in an "Email address
         * not verified" error. </p> </li> <li> <p>If your account is still in the Amazon
         * SES sandbox, you may only send to verified addresses or domains, or to email
         * addresses associated with the Amazon SES Mailbox Simulator. For more
         * information, see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/verify-addresses-and-domains.html">Verifying
         * Email Addresses and Domains</a> in the <i>Amazon SES Developer Guide.</i> </p>
         * </li> <li> <p>The maximum message size is 10 MB.</p> </li> <li> <p>The message
         * must include at least one recipient email address. The recipient address can be
         * a To: address, a CC: address, or a BCC: address. If a recipient email address is
         * invalid (that is, it is not in the format
         * <i>UserName@[SubDomain.]Domain.TopLevelDomain</i>), the entire message will be
         * rejected, even if the message contains other recipients that are valid.</p>
         * </li> <li> <p>The message may not include more than 50 recipients, across the
         * To:, CC: and BCC: fields. If you need to send an email message to a larger
         * audience, you can divide your recipient list into groups of 50 or fewer, and
         * then call the <code>SendEmail</code> operation several times to send the message
         * to each group.</p> </li> </ul>  <p>For every message that you send,
         * the total number of recipients (including each recipient in the To:, CC: and
         * BCC: fields) is counted against the maximum number of emails you can send in a
         * 24-hour period (your <i>sending quota</i>). For more information about sending
         * quotas in Amazon SES, see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/manage-sending-limits.html">Managing
         * Your Amazon SES Sending Limits</a> in the <i>Amazon SES Developer Guide.</i>
         * </p> <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/SendEmail">AWS API
         * Reference</a></p>
         */
        virtual Model::SendEmailOutcome SendEmail(const Model::SendEmailRequest& request) const;


        /**
         * <p>Composes an email message and immediately queues it for sending.</p> <p>This
         * operation is more flexible than the <code>SendEmail</code> API operation. When
         * you use the <code>SendRawEmail</code> operation, you can specify the headers of
         * the message as well as its content. This flexibility is useful, for example,
         * when you want to send a multipart MIME email (such a message that contains both
         * a text and an HTML version). You can also use this operation to send messages
         * that include attachments.</p> <p>The <code>SendRawEmail</code> operation has the
         * following requirements:</p> <ul> <li> <p>You can only send email from <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/verify-addresses-and-domains.html">verified
         * email addresses or domains</a>. If you try to send email from an address that
         * isn't verified, the operation results in an "Email address not verified"
         * error.</p> </li> <li> <p>If your account is still in the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/request-production-access.html">Amazon
         * SES sandbox</a>, you can only send email to other verified addresses in your
         * account, or to addresses that are associated with the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/mailbox-simulator.html">Amazon
         * SES mailbox simulator</a>.</p> </li> <li> <p>The maximum message size, including
         * attachments, is 10 MB.</p> </li> <li> <p>Each message has to include at least
         * one recipient address. A recipient address includes any address on the To:, CC:,
         * or BCC: lines.</p> </li> <li> <p>If you send a single message to more than one
         * recipient address, and one of the recipient addresses isn't in a valid format
         * (that is, it's not in the format
         * <i>UserName@[SubDomain.]Domain.TopLevelDomain</i>), Amazon SES rejects the
         * entire message, even if the other addresses are valid.</p> </li> <li> <p>Each
         * message can include up to 50 recipient addresses across the To:, CC:, or BCC:
         * lines. If you need to send a single message to more than 50 recipients, you have
         * to split the list of recipient addresses into groups of less than 50 recipients,
         * and send separate messages to each group.</p> </li> <li> <p>Amazon SES allows
         * you to specify 8-bit Content-Transfer-Encoding for MIME message parts. However,
         * if Amazon SES has to modify the contents of your message (for example, if you
         * use open and click tracking), 8-bit content isn't preserved. For this reason, we
         * highly recommend that you encode all content that isn't 7-bit ASCII. For more
         * information, see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/send-email-raw.html#send-email-mime-encoding">MIME
         * Encoding</a> in the <i>Amazon SES Developer Guide</i>.</p> </li> </ul>
         * <p>Additionally, keep the following considerations in mind when using the
         * <code>SendRawEmail</code> operation:</p> <ul> <li> <p>Although you can customize
         * the message headers when using the <code>SendRawEmail</code> operation, Amazon
         * SES will automatically apply its own <code>Message-ID</code> and
         * <code>Date</code> headers; if you passed these headers when creating the
         * message, they will be overwritten by the values that Amazon SES provides.</p>
         * </li> <li> <p>If you are using sending authorization to send on behalf of
         * another user, <code>SendRawEmail</code> enables you to specify the cross-account
         * identity for the email's Source, From, and Return-Path parameters in one of two
         * ways: you can pass optional parameters <code>SourceArn</code>,
         * <code>FromArn</code>, and/or <code>ReturnPathArn</code> to the API, or you can
         * include the following X-headers in the header of your raw email:</p> <ul> <li>
         * <p> <code>X-SES-SOURCE-ARN</code> </p> </li> <li> <p>
         * <code>X-SES-FROM-ARN</code> </p> </li> <li> <p>
         * <code>X-SES-RETURN-PATH-ARN</code> </p> </li> </ul>  <p>Don't include
         * these X-headers in the DKIM signature. Amazon SES removes these before it sends
         * the email.</p>  <p>If you only specify the
         * <code>SourceIdentityArn</code> parameter, Amazon SES sets the From and
         * Return-Path addresses to the same identity that you specified.</p> <p>For more
         * information about sending authorization, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/sending-authorization.html">Using
         * Sending Authorization with Amazon SES</a> in the <i>Amazon SES Developer
         * Guide.</i> </p> </li> <li> <p>For every message that you send, the total number
         * of recipients (including each recipient in the To:, CC: and BCC: fields) is
         * counted against the maximum number of emails you can send in a 24-hour period
         * (your <i>sending quota</i>). For more information about sending quotas in Amazon
         * SES, see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/manage-sending-limits.html">Managing
         * Your Amazon SES Sending Limits</a> in the <i>Amazon SES Developer Guide.</i>
         * </p> </li> </ul><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/SendRawEmail">AWS
         * API Reference</a></p>
         */
        virtual Model::SendRawEmailOutcome SendRawEmail(const Model::SendRawEmailRequest& request) const;


        /**
         * <p>Composes an email message using an email template and immediately queues it
         * for sending.</p> <p>In order to send email using the
         * <code>SendTemplatedEmail</code> operation, your call to the API must meet the
         * following requirements:</p> <ul> <li> <p>The call must refer to an existing
         * email template. You can create email templates using the <a>CreateTemplate</a>
         * operation.</p> </li> <li> <p>The message must be sent from a verified email
         * address or domain.</p> </li> <li> <p>If your account is still in the Amazon SES
         * sandbox, you may only send to verified addresses or domains, or to email
         * addresses associated with the Amazon SES Mailbox Simulator. For more
         * information, see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/verify-addresses-and-domains.html">Verifying
         * Email Addresses and Domains</a> in the <i>Amazon SES Developer Guide.</i> </p>
         * </li> <li> <p>The maximum message size is 10 MB.</p> </li> <li> <p>Calls to the
         * <code>SendTemplatedEmail</code> operation may only include one
         * <code>Destination</code> parameter. A destination is a set of recipients who
         * will receive the same version of the email. The <code>Destination</code>
         * parameter can include up to 50 recipients, across the To:, CC: and BCC:
         * fields.</p> </li> <li> <p>The <code>Destination</code> parameter must include at
         * least one recipient email address. The recipient address can be a To: address, a
         * CC: address, or a BCC: address. If a recipient email address is invalid (that
         * is, it is not in the format <i>UserName@[SubDomain.]Domain.TopLevelDomain</i>),
         * the entire message will be rejected, even if the message contains other
         * recipients that are valid.</p> </li> </ul>  <p>If your call to the
         * <code>SendTemplatedEmail</code> operation includes all of the required
         * parameters, Amazon SES accepts it and returns a Message ID. However, if Amazon
         * SES can't render the email because the template contains errors, it doesn't send
         * the email. Additionally, because it already accepted the message, Amazon SES
         * doesn't return a message stating that it was unable to send the email.</p>
         * <p>For these reasons, we highly recommend that you set up Amazon SES to send you
         * notifications when Rendering Failure events occur. For more information, see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/send-personalized-email-api.html">Sending
         * Personalized Email Using the Amazon SES API</a> in the <i>Amazon Simple Email
         * Service Developer Guide</i>.</p> <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/SendTemplatedEmail">AWS
         * API Reference</a></p>
         */
        virtual Model::SendTemplatedEmailOutcome SendTemplatedEmail(const Model::SendTemplatedEmailRequest& request) const;


        /**
         * <p>Sets the specified receipt rule set as the active receipt rule set.</p>
         *  <p>To disable your email-receiving through Amazon SES completely, you can
         * call this API with RuleSetName set to null.</p>  <p>For information about
         * managing receipt rule sets, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-managing-receipt-rule-sets.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/SetActiveReceiptRuleSet">AWS
         * API Reference</a></p>
         */
        virtual Model::SetActiveReceiptRuleSetOutcome SetActiveReceiptRuleSet(const Model::SetActiveReceiptRuleSetRequest& request) const;


        /**
         * <p>Enables or disables Easy DKIM signing of email sent from an identity. If Easy
         * DKIM signing is enabled for a domain, then Amazon SES uses DKIM to sign all
         * email that it sends from addresses on that domain. If Easy DKIM signing is
         * enabled for an email address, then Amazon SES uses DKIM to sign all email it
         * sends from that address.</p>  <p>For email addresses (for example,
         * <code>user@example.com</code>), you can only enable DKIM signing if the
         * corresponding domain (in this case, <code>example.com</code>) has been set up to
         * use Easy DKIM.</p>  <p>You can enable DKIM signing for an identity at any
         * time after you start the verification process for the identity, even if the
         * verification process isn't complete. </p> <p>You can execute this operation no
         * more than once per second.</p> <p>For more information about Easy DKIM signing,
         * go to the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/easy-dkim.html">Amazon
         * SES Developer Guide</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/SetIdentityDkimEnabled">AWS
         * API Reference</a></p>
         */
        virtual Model::SetIdentityDkimEnabledOutcome SetIdentityDkimEnabled(const Model::SetIdentityDkimEnabledRequest& request) const;


        /**
         * <p>Given an identity (an email address or a domain), enables or disables whether
         * Amazon SES forwards bounce and complaint notifications as email. Feedback
         * forwarding can only be disabled when Amazon Simple Notification Service (Amazon
         * SNS) topics are specified for both bounces and complaints.</p> 
         * <p>Feedback forwarding does not apply to delivery notifications. Delivery
         * notifications are only available through Amazon SNS.</p>  <p>You can
         * execute this operation no more than once per second.</p> <p>For more information
         * about using notifications with Amazon SES, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/notifications.html">Amazon
         * SES Developer Guide</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/SetIdentityFeedbackForwardingEnabled">AWS
         * API Reference</a></p>
         */
        virtual Model::SetIdentityFeedbackForwardingEnabledOutcome SetIdentityFeedbackForwardingEnabled(const Model::SetIdentityFeedbackForwardingEnabledRequest& request) const;


        /**
         * <p>Given an identity (an email address or a domain), sets whether Amazon SES
         * includes the original email headers in the Amazon Simple Notification Service
         * (Amazon SNS) notifications of a specified type.</p> <p>You can execute this
         * operation no more than once per second.</p> <p>For more information about using
         * notifications with Amazon SES, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/notifications.html">Amazon
         * SES Developer Guide</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/SetIdentityHeadersInNotificationsEnabled">AWS
         * API Reference</a></p>
         */
        virtual Model::SetIdentityHeadersInNotificationsEnabledOutcome SetIdentityHeadersInNotificationsEnabled(const Model::SetIdentityHeadersInNotificationsEnabledRequest& request) const;


        /**
         * <p>Enables or disables the custom MAIL FROM domain setup for a verified identity
         * (an email address or a domain).</p>  <p>To send emails using the
         * specified MAIL FROM domain, you must add an MX record to your MAIL FROM domain's
         * DNS settings. If you want your emails to pass Sender Policy Framework (SPF)
         * checks, you must also add or update an SPF record. For more information, see the
         * <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/mail-from-set.html">Amazon
         * SES Developer Guide</a>.</p>  <p>You can execute this operation no
         * more than once per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/SetIdentityMailFromDomain">AWS
         * API Reference</a></p>
         */
        virtual Model::SetIdentityMailFromDomainOutcome SetIdentityMailFromDomain(const Model::SetIdentityMailFromDomainRequest& request) const;


        /**
         * <p>Sets an Amazon Simple Notification Service (Amazon SNS) topic to use when
         * delivering notifications. When you use this operation, you specify a verified
         * identity, such as an email address or domain. When you send an email that uses
         * the chosen identity in the Source field, Amazon SES sends notifications to the
         * topic you specified. You can send bounce, complaint, or delivery notifications
         * (or any combination of the three) to the Amazon SNS topic that you specify.</p>
         * <p>You can execute this operation no more than once per second.</p> <p>For more
         * information about feedback notification, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/notifications.html">Amazon
         * SES Developer Guide</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/SetIdentityNotificationTopic">AWS
         * API Reference</a></p>
         */
        virtual Model::SetIdentityNotificationTopicOutcome SetIdentityNotificationTopic(const Model::SetIdentityNotificationTopicRequest& request) const;


        /**
         * <p>Sets the position of the specified receipt rule in the receipt rule set.</p>
         * <p>For information about managing receipt rules, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-managing-receipt-rules.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/SetReceiptRulePosition">AWS
         * API Reference</a></p>
         */
        virtual Model::SetReceiptRulePositionOutcome SetReceiptRulePosition(const Model::SetReceiptRulePositionRequest& request) const;


        /**
         * <p>Creates a preview of the MIME content of an email when provided with a
         * template and a set of replacement data.</p> <p>You can execute this operation no
         * more than once per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/TestRenderTemplate">AWS
         * API Reference</a></p>
         */
        virtual Model::TestRenderTemplateOutcome TestRenderTemplate(const Model::TestRenderTemplateRequest& request) const;


        /**
         * <p>Enables or disables email sending across your entire Amazon SES account in
         * the current AWS Region. You can use this operation in conjunction with Amazon
         * CloudWatch alarms to temporarily pause email sending across your Amazon SES
         * account in a given AWS Region when reputation metrics (such as your bounce or
         * complaint rates) reach certain thresholds.</p> <p>You can execute this operation
         * no more than once per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/UpdateAccountSendingEnabled">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateAccountSendingEnabledOutcome UpdateAccountSendingEnabled(const Model::UpdateAccountSendingEnabledRequest& request) const;


        /**
         * <p>Updates the event destination of a configuration set. Event destinations are
         * associated with configuration sets, which enable you to publish email sending
         * events to Amazon CloudWatch, Amazon Kinesis Firehose, or Amazon Simple
         * Notification Service (Amazon SNS). For information about using configuration
         * sets, see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/monitor-sending-activity.html">Monitoring
         * Your Amazon SES Sending Activity</a> in the <i>Amazon SES Developer Guide.</i>
         * </p>  <p>When you create or update an event destination, you must provide
         * one, and only one, destination. The destination can be Amazon CloudWatch, Amazon
         * Kinesis Firehose, or Amazon Simple Notification Service (Amazon SNS).</p>
         *  <p>You can execute this operation no more than once per
         * second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/UpdateConfigurationSetEventDestination">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateConfigurationSetEventDestinationOutcome UpdateConfigurationSetEventDestination(const Model::UpdateConfigurationSetEventDestinationRequest& request) const;


        /**
         * <p>Enables or disables the publishing of reputation metrics for emails sent
         * using a specific configuration set in a given AWS Region. Reputation metrics
         * include bounce and complaint rates. These metrics are published to Amazon
         * CloudWatch. By using CloudWatch, you can create alarms when bounce or complaint
         * rates exceed certain thresholds.</p> <p>You can execute this operation no more
         * than once per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/UpdateConfigurationSetReputationMetricsEnabled">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateConfigurationSetReputationMetricsEnabledOutcome UpdateConfigurationSetReputationMetricsEnabled(const Model::UpdateConfigurationSetReputationMetricsEnabledRequest& request) const;


        /**
         * <p>Enables or disables email sending for messages sent using a specific
         * configuration set in a given AWS Region. You can use this operation in
         * conjunction with Amazon CloudWatch alarms to temporarily pause email sending for
         * a configuration set when the reputation metrics for that configuration set (such
         * as your bounce on complaint rate) exceed certain thresholds.</p> <p>You can
         * execute this operation no more than once per second.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/UpdateConfigurationSetSendingEnabled">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateConfigurationSetSendingEnabledOutcome UpdateConfigurationSetSendingEnabled(const Model::UpdateConfigurationSetSendingEnabledRequest& request) const;


        /**
         * <p>Modifies an association between a configuration set and a custom domain for
         * open and click event tracking. </p> <p>By default, images and links used for
         * tracking open and click events are hosted on domains operated by Amazon SES. You
         * can configure a subdomain of your own to handle these events. For information
         * about using custom domains, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/configure-custom-open-click-domains.html">Amazon
         * SES Developer Guide</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/UpdateConfigurationSetTrackingOptions">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateConfigurationSetTrackingOptionsOutcome UpdateConfigurationSetTrackingOptions(const Model::UpdateConfigurationSetTrackingOptionsRequest& request) const;


        /**
         * <p>Updates an existing custom verification email template.</p> <p>For more
         * information about custom verification email templates, see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/custom-verification-emails.html">Using
         * Custom Verification Email Templates</a> in the <i>Amazon SES Developer
         * Guide</i>.</p> <p>You can execute this operation no more than once per
         * second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/UpdateCustomVerificationEmailTemplate">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateCustomVerificationEmailTemplateOutcome UpdateCustomVerificationEmailTemplate(const Model::UpdateCustomVerificationEmailTemplateRequest& request) const;


        /**
         * <p>Updates a receipt rule.</p> <p>For information about managing receipt rules,
         * see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/receiving-email-managing-receipt-rules.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/UpdateReceiptRule">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateReceiptRuleOutcome UpdateReceiptRule(const Model::UpdateReceiptRuleRequest& request) const;


        /**
         * <p>Updates an email template. Email templates enable you to send personalized
         * email to one or more destinations in a single API operation. For more
         * information, see the <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/send-personalized-email-api.html">Amazon
         * SES Developer Guide</a>.</p> <p>You can execute this operation no more than once
         * per second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/UpdateTemplate">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateTemplateOutcome UpdateTemplate(const Model::UpdateTemplateRequest& request) const;


        /**
         * <p>Returns a set of DKIM tokens for a domain identity.</p>  <p>When
         * you execute the <code>VerifyDomainDkim</code> operation, the domain that you
         * specify is added to the list of identities that are associated with your
         * account. This is true even if you haven't already associated the domain with
         * your account by using the <code>VerifyDomainIdentity</code> operation. However,
         * you can't send email from the domain until you either successfully <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/verify-domains.html">verify
         * it</a> or you successfully <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/easy-dkim.html">set
         * up DKIM for it</a>.</p>  <p>You use the tokens that are generated by
         * this operation to create CNAME records. When Amazon SES detects that you've
         * added these records to the DNS configuration for a domain, you can start sending
         * email from that domain. You can start sending email even if you haven't added
         * the TXT record provided by the VerifyDomainIdentity operation to the DNS
         * configuration for your domain. All email that you send from the domain is
         * authenticated using DKIM.</p> <p>To create the CNAME records for DKIM
         * authentication, use the following values:</p> <ul> <li> <p> <b>Name</b>:
         * <i>token</i>._domainkey.<i>example.com</i> </p> </li> <li> <p> <b>Type</b>:
         * CNAME</p> </li> <li> <p> <b>Value</b>: <i>token</i>.dkim.amazonses.com</p> </li>
         * </ul> <p>In the preceding example, replace <i>token</i> with one of the tokens
         * that are generated when you execute this operation. Replace <i>example.com</i>
         * with your domain. Repeat this process for each token that's generated by this
         * operation.</p> <p>You can execute this operation no more than once per
         * second.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/VerifyDomainDkim">AWS
         * API Reference</a></p>
         */
        virtual Model::VerifyDomainDkimOutcome VerifyDomainDkim(const Model::VerifyDomainDkimRequest& request) const;


        /**
         * <p>Adds a domain to the list of identities for your Amazon SES account in the
         * current AWS Region and attempts to verify it. For more information about
         * verifying domains, see <a
         * href="https://docs.aws.amazon.com/ses/latest/DeveloperGuide/verify-addresses-and-domains.html">Verifying
         * Email Addresses and Domains</a> in the <i>Amazon SES Developer Guide.</i> </p>
         * <p>You can execute this operation no more than once per second.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/VerifyDomainIdentity">AWS
         * API Reference</a></p>
         */
        virtual Model::VerifyDomainIdentityOutcome VerifyDomainIdentity(const Model::VerifyDomainIdentityRequest& request) const;


        /**
         * <p>Deprecated. Use the <code>VerifyEmailIdentity</code> operation to verify a
         * new email address.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/VerifyEmailAddress">AWS
         * API Reference</a></p>
         */
        virtual Model::VerifyEmailAddressOutcome VerifyEmailAddress(const Model::VerifyEmailAddressRequest& request) const;


        /**
         * <p>Adds an email address to the list of identities for your Amazon SES account
         * in the current AWS region and attempts to verify it. As a result of executing
         * this operation, a verification email is sent to the specified address.</p>
         * <p>You can execute this operation no more than once per second.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/email-2010-12-01/VerifyEmailIdentity">AWS
         * API Reference</a></p>
         */
        virtual Model::VerifyEmailIdentityOutcome VerifyEmailIdentity(const Model::VerifyEmailIdentityRequest& request) const;



        void OverrideEndpoint(const Aws::String& endpoint);
        std::shared_ptr<SESEndpointProviderBase>& accessEndpointProvider();
  private:
        void init(const SESClientConfiguration& clientConfiguration);

        SESClientConfiguration m_clientConfiguration;
        std::shared_ptr<Aws::Utils::Threading::Executor> m_executor;
        std::shared_ptr<SESEndpointProviderBase> m_endpointProvider;
  };

} // namespace SES
} // namespace Aws
