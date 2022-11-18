﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/events/CloudWatchEvents_EXPORTS.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/client/AWSClient.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/events/CloudWatchEventsServiceClientModel.h>
#include <aws/events/CloudWatchEventsLegacyAsyncMacros.h>

namespace Aws
{
namespace CloudWatchEvents
{
  /**
   * <p>Amazon EventBridge helps you to respond to state changes in your Amazon Web
   * Services resources. When your resources change state, they automatically send
   * events to an event stream. You can create rules that match selected events in
   * the stream and route them to targets to take action. You can also use rules to
   * take action on a predetermined schedule. For example, you can configure rules
   * to:</p> <ul> <li> <p>Automatically invoke an Lambda function to update DNS
   * entries when an event notifies you that Amazon EC2 instance enters the running
   * state.</p> </li> <li> <p>Direct specific API records from CloudTrail to an
   * Amazon Kinesis data stream for detailed analysis of potential security or
   * availability risks.</p> </li> <li> <p>Periodically invoke a built-in target to
   * create a snapshot of an Amazon EBS volume.</p> </li> </ul> <p>For more
   * information about the features of Amazon EventBridge, see the <a
   * href="https://docs.aws.amazon.com/eventbridge/latest/userguide">Amazon
   * EventBridge User Guide</a>.</p>
   */
  class AWS_CLOUDWATCHEVENTS_API CloudWatchEventsClient : public Aws::Client::AWSJsonClient
  {
    public:
      typedef Aws::Client::AWSJsonClient BASECLASS;
      static const char* SERVICE_NAME;
      static const char* ALLOCATION_TAG;

       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CloudWatchEventsClient(const Aws::CloudWatchEvents::CloudWatchEventsClientConfiguration& clientConfiguration = Aws::CloudWatchEvents::CloudWatchEventsClientConfiguration(),
                               std::shared_ptr<CloudWatchEventsEndpointProviderBase> endpointProvider = Aws::MakeShared<CloudWatchEventsEndpointProvider>(ALLOCATION_TAG));

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CloudWatchEventsClient(const Aws::Auth::AWSCredentials& credentials,
                               std::shared_ptr<CloudWatchEventsEndpointProviderBase> endpointProvider = Aws::MakeShared<CloudWatchEventsEndpointProvider>(ALLOCATION_TAG),
                               const Aws::CloudWatchEvents::CloudWatchEventsClientConfiguration& clientConfiguration = Aws::CloudWatchEvents::CloudWatchEventsClientConfiguration());

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        CloudWatchEventsClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                               std::shared_ptr<CloudWatchEventsEndpointProviderBase> endpointProvider = Aws::MakeShared<CloudWatchEventsEndpointProvider>(ALLOCATION_TAG),
                               const Aws::CloudWatchEvents::CloudWatchEventsClientConfiguration& clientConfiguration = Aws::CloudWatchEvents::CloudWatchEventsClientConfiguration());


        /* Legacy constructors due deprecation */
       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CloudWatchEventsClient(const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CloudWatchEventsClient(const Aws::Auth::AWSCredentials& credentials,
                               const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        CloudWatchEventsClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                               const Aws::Client::ClientConfiguration& clientConfiguration);

        /* End of legacy constructors due deprecation */
        virtual ~CloudWatchEventsClient();


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
         * <p>Activates a partner event source that has been deactivated. Once activated,
         * your matching event bus will start receiving events from the event
         * source.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/ActivateEventSource">AWS
         * API Reference</a></p>
         */
        virtual Model::ActivateEventSourceOutcome ActivateEventSource(const Model::ActivateEventSourceRequest& request) const;


        /**
         * <p>Cancels the specified replay.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/CancelReplay">AWS
         * API Reference</a></p>
         */
        virtual Model::CancelReplayOutcome CancelReplay(const Model::CancelReplayRequest& request) const;


        /**
         * <p>Creates an API destination, which is an HTTP invocation endpoint configured
         * as a target for events.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/CreateApiDestination">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateApiDestinationOutcome CreateApiDestination(const Model::CreateApiDestinationRequest& request) const;


        /**
         * <p>Creates an archive of events with the specified settings. When you create an
         * archive, incoming events might not immediately start being sent to the archive.
         * Allow a short period of time for changes to take effect. If you do not specify a
         * pattern to filter events sent to the archive, all events are sent to the archive
         * except replayed events. Replayed events are not sent to an
         * archive.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/CreateArchive">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateArchiveOutcome CreateArchive(const Model::CreateArchiveRequest& request) const;


        /**
         * <p>Creates a connection. A connection defines the authorization type and
         * credentials to use for authorization with an API destination HTTP
         * endpoint.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/CreateConnection">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateConnectionOutcome CreateConnection(const Model::CreateConnectionRequest& request) const;


        /**
         * <p>Creates a new event bus within your account. This can be a custom event bus
         * which you can use to receive events from your custom applications and services,
         * or it can be a partner event bus which can be matched to a partner event
         * source.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/CreateEventBus">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateEventBusOutcome CreateEventBus(const Model::CreateEventBusRequest& request) const;


        /**
         * <p>Called by an SaaS partner to create a partner event source. This operation is
         * not used by Amazon Web Services customers.</p> <p>Each partner event source can
         * be used by one Amazon Web Services account to create a matching partner event
         * bus in that Amazon Web Services account. A SaaS partner must create one partner
         * event source for each Amazon Web Services account that wants to receive those
         * event types. </p> <p>A partner event source creates events based on resources
         * within the SaaS partner's service or application.</p> <p>An Amazon Web Services
         * account that creates a partner event bus that matches the partner event source
         * can use that event bus to receive events from the partner, and then process them
         * using Amazon Web Services Events rules and targets.</p> <p>Partner event source
         * names follow this format:</p> <p> <code>
         * <i>partner_name</i>/<i>event_namespace</i>/<i>event_name</i> </code> </p> <p>
         * <i>partner_name</i> is determined during partner registration and identifies the
         * partner to Amazon Web Services customers. <i>event_namespace</i> is determined
         * by the partner and is a way for the partner to categorize their events.
         * <i>event_name</i> is determined by the partner, and should uniquely identify an
         * event-generating resource within the partner system. The combination of
         * <i>event_namespace</i> and <i>event_name</i> should help Amazon Web Services
         * customers decide whether to create an event bus to receive these
         * events.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/CreatePartnerEventSource">AWS
         * API Reference</a></p>
         */
        virtual Model::CreatePartnerEventSourceOutcome CreatePartnerEventSource(const Model::CreatePartnerEventSourceRequest& request) const;


        /**
         * <p>You can use this operation to temporarily stop receiving events from the
         * specified partner event source. The matching event bus is not deleted. </p>
         * <p>When you deactivate a partner event source, the source goes into PENDING
         * state. If it remains in PENDING state for more than two weeks, it is
         * deleted.</p> <p>To activate a deactivated partner event source, use <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/APIReference/API_ActivateEventSource.html">ActivateEventSource</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DeactivateEventSource">AWS
         * API Reference</a></p>
         */
        virtual Model::DeactivateEventSourceOutcome DeactivateEventSource(const Model::DeactivateEventSourceRequest& request) const;


        /**
         * <p>Removes all authorization parameters from the connection. This lets you
         * remove the secret from the connection so you can reuse it without having to
         * create a new connection.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DeauthorizeConnection">AWS
         * API Reference</a></p>
         */
        virtual Model::DeauthorizeConnectionOutcome DeauthorizeConnection(const Model::DeauthorizeConnectionRequest& request) const;


        /**
         * <p>Deletes the specified API destination.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DeleteApiDestination">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteApiDestinationOutcome DeleteApiDestination(const Model::DeleteApiDestinationRequest& request) const;


        /**
         * <p>Deletes the specified archive.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DeleteArchive">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteArchiveOutcome DeleteArchive(const Model::DeleteArchiveRequest& request) const;


        /**
         * <p>Deletes a connection.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DeleteConnection">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteConnectionOutcome DeleteConnection(const Model::DeleteConnectionRequest& request) const;


        /**
         * <p>Deletes the specified custom event bus or partner event bus. All rules
         * associated with this event bus need to be deleted. You can't delete your
         * account's default event bus.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DeleteEventBus">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteEventBusOutcome DeleteEventBus(const Model::DeleteEventBusRequest& request) const;


        /**
         * <p>This operation is used by SaaS partners to delete a partner event source.
         * This operation is not used by Amazon Web Services customers.</p> <p>When you
         * delete an event source, the status of the corresponding partner event bus in the
         * Amazon Web Services customer account becomes DELETED.</p> <p/><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DeletePartnerEventSource">AWS
         * API Reference</a></p>
         */
        virtual Model::DeletePartnerEventSourceOutcome DeletePartnerEventSource(const Model::DeletePartnerEventSourceRequest& request) const;


        /**
         * <p>Deletes the specified rule.</p> <p>Before you can delete the rule, you must
         * remove all targets, using <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/APIReference/API_RemoveTargets.html">RemoveTargets</a>.</p>
         * <p>When you delete a rule, incoming events might continue to match to the
         * deleted rule. Allow a short period of time for changes to take effect.</p> <p>If
         * you call delete rule multiple times for the same rule, all calls will succeed.
         * When you call delete rule for a non-existent custom eventbus,
         * <code>ResourceNotFoundException</code> is returned.</p> <p>Managed rules are
         * rules created and managed by another Amazon Web Services service on your behalf.
         * These rules are created by those other Amazon Web Services services to support
         * functionality in those services. You can delete these rules using the
         * <code>Force</code> option, but you should do so only if you are sure the other
         * service is not still using that rule.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DeleteRule">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteRuleOutcome DeleteRule(const Model::DeleteRuleRequest& request) const;


        /**
         * <p>Retrieves details about an API destination.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DescribeApiDestination">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeApiDestinationOutcome DescribeApiDestination(const Model::DescribeApiDestinationRequest& request) const;


        /**
         * <p>Retrieves details about an archive.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DescribeArchive">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeArchiveOutcome DescribeArchive(const Model::DescribeArchiveRequest& request) const;


        /**
         * <p>Retrieves details about a connection.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DescribeConnection">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeConnectionOutcome DescribeConnection(const Model::DescribeConnectionRequest& request) const;


        /**
         * <p>Displays details about an event bus in your account. This can include the
         * external Amazon Web Services accounts that are permitted to write events to your
         * default event bus, and the associated policy. For custom event buses and partner
         * event buses, it displays the name, ARN, policy, state, and creation time.</p>
         * <p> To enable your account to receive events from other accounts on its default
         * event bus, use <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/APIReference/API_PutPermission.html">PutPermission</a>.</p>
         * <p>For more information about partner event buses, see <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/APIReference/API_CreateEventBus.html">CreateEventBus</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DescribeEventBus">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeEventBusOutcome DescribeEventBus(const Model::DescribeEventBusRequest& request) const;


        /**
         * <p>This operation lists details about a partner event source that is shared with
         * your account.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DescribeEventSource">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeEventSourceOutcome DescribeEventSource(const Model::DescribeEventSourceRequest& request) const;


        /**
         * <p>An SaaS partner can use this operation to list details about a partner event
         * source that they have created. Amazon Web Services customers do not use this
         * operation. Instead, Amazon Web Services customers can use <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/APIReference/API_DescribeEventSource.html">DescribeEventSource</a>
         * to see details about a partner event source that is shared with
         * them.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DescribePartnerEventSource">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribePartnerEventSourceOutcome DescribePartnerEventSource(const Model::DescribePartnerEventSourceRequest& request) const;


        /**
         * <p>Retrieves details about a replay. Use <code>DescribeReplay</code> to
         * determine the progress of a running replay. A replay processes events to replay
         * based on the time in the event, and replays them using 1 minute intervals. If
         * you use <code>StartReplay</code> and specify an <code>EventStartTime</code> and
         * an <code>EventEndTime</code> that covers a 20 minute time range, the events are
         * replayed from the first minute of that 20 minute range first. Then the events
         * from the second minute are replayed. You can use <code>DescribeReplay</code> to
         * determine the progress of a replay. The value returned for
         * <code>EventLastReplayedTime</code> indicates the time within the specified time
         * range associated with the last event replayed.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DescribeReplay">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeReplayOutcome DescribeReplay(const Model::DescribeReplayRequest& request) const;


        /**
         * <p>Describes the specified rule.</p> <p>DescribeRule does not list the targets
         * of a rule. To see the targets associated with a rule, use <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/APIReference/API_ListTargetsByRule.html">ListTargetsByRule</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DescribeRule">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeRuleOutcome DescribeRule(const Model::DescribeRuleRequest& request) const;


        /**
         * <p>Disables the specified rule. A disabled rule won't match any events, and
         * won't self-trigger if it has a schedule expression.</p> <p>When you disable a
         * rule, incoming events might continue to match to the disabled rule. Allow a
         * short period of time for changes to take effect.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/DisableRule">AWS
         * API Reference</a></p>
         */
        virtual Model::DisableRuleOutcome DisableRule(const Model::DisableRuleRequest& request) const;


        /**
         * <p>Enables the specified rule. If the rule does not exist, the operation
         * fails.</p> <p>When you enable a rule, incoming events might not immediately
         * start matching to a newly enabled rule. Allow a short period of time for changes
         * to take effect.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/EnableRule">AWS
         * API Reference</a></p>
         */
        virtual Model::EnableRuleOutcome EnableRule(const Model::EnableRuleRequest& request) const;


        /**
         * <p>Retrieves a list of API destination in the account in the current
         * Region.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/ListApiDestinations">AWS
         * API Reference</a></p>
         */
        virtual Model::ListApiDestinationsOutcome ListApiDestinations(const Model::ListApiDestinationsRequest& request) const;


        /**
         * <p>Lists your archives. You can either list all the archives or you can provide
         * a prefix to match to the archive names. Filter parameters are
         * exclusive.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/ListArchives">AWS
         * API Reference</a></p>
         */
        virtual Model::ListArchivesOutcome ListArchives(const Model::ListArchivesRequest& request) const;


        /**
         * <p>Retrieves a list of connections from the account.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/ListConnections">AWS
         * API Reference</a></p>
         */
        virtual Model::ListConnectionsOutcome ListConnections(const Model::ListConnectionsRequest& request) const;


        /**
         * <p>Lists all the event buses in your account, including the default event bus,
         * custom event buses, and partner event buses.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/ListEventBuses">AWS
         * API Reference</a></p>
         */
        virtual Model::ListEventBusesOutcome ListEventBuses(const Model::ListEventBusesRequest& request) const;


        /**
         * <p>You can use this to see all the partner event sources that have been shared
         * with your Amazon Web Services account. For more information about partner event
         * sources, see <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/APIReference/API_CreateEventBus.html">CreateEventBus</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/ListEventSources">AWS
         * API Reference</a></p>
         */
        virtual Model::ListEventSourcesOutcome ListEventSources(const Model::ListEventSourcesRequest& request) const;


        /**
         * <p>An SaaS partner can use this operation to display the Amazon Web Services
         * account ID that a particular partner event source name is associated with. This
         * operation is not used by Amazon Web Services customers.</p><p><h3>See Also:</h3>
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/ListPartnerEventSourceAccounts">AWS
         * API Reference</a></p>
         */
        virtual Model::ListPartnerEventSourceAccountsOutcome ListPartnerEventSourceAccounts(const Model::ListPartnerEventSourceAccountsRequest& request) const;


        /**
         * <p>An SaaS partner can use this operation to list all the partner event source
         * names that they have created. This operation is not used by Amazon Web Services
         * customers.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/ListPartnerEventSources">AWS
         * API Reference</a></p>
         */
        virtual Model::ListPartnerEventSourcesOutcome ListPartnerEventSources(const Model::ListPartnerEventSourcesRequest& request) const;


        /**
         * <p>Lists your replays. You can either list all the replays or you can provide a
         * prefix to match to the replay names. Filter parameters are
         * exclusive.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/ListReplays">AWS
         * API Reference</a></p>
         */
        virtual Model::ListReplaysOutcome ListReplays(const Model::ListReplaysRequest& request) const;


        /**
         * <p>Lists the rules for the specified target. You can see which of the rules in
         * Amazon EventBridge can invoke a specific target in your account.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/ListRuleNamesByTarget">AWS
         * API Reference</a></p>
         */
        virtual Model::ListRuleNamesByTargetOutcome ListRuleNamesByTarget(const Model::ListRuleNamesByTargetRequest& request) const;


        /**
         * <p>Lists your Amazon EventBridge rules. You can either list all the rules or you
         * can provide a prefix to match to the rule names.</p> <p>ListRules does not list
         * the targets of a rule. To see the targets associated with a rule, use <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/APIReference/API_ListTargetsByRule.html">ListTargetsByRule</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/ListRules">AWS
         * API Reference</a></p>
         */
        virtual Model::ListRulesOutcome ListRules(const Model::ListRulesRequest& request) const;


        /**
         * <p>Displays the tags associated with an EventBridge resource. In EventBridge,
         * rules and event buses can be tagged.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/ListTagsForResource">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTagsForResourceOutcome ListTagsForResource(const Model::ListTagsForResourceRequest& request) const;


        /**
         * <p>Lists the targets assigned to the specified rule.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/ListTargetsByRule">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTargetsByRuleOutcome ListTargetsByRule(const Model::ListTargetsByRuleRequest& request) const;


        /**
         * <p>Sends custom events to Amazon EventBridge so that they can be matched to
         * rules.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/PutEvents">AWS
         * API Reference</a></p>
         */
        virtual Model::PutEventsOutcome PutEvents(const Model::PutEventsRequest& request) const;


        /**
         * <p>This is used by SaaS partners to write events to a customer's partner event
         * bus. Amazon Web Services customers do not use this operation.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/PutPartnerEvents">AWS
         * API Reference</a></p>
         */
        virtual Model::PutPartnerEventsOutcome PutPartnerEvents(const Model::PutPartnerEventsRequest& request) const;


        /**
         * <p>Running <code>PutPermission</code> permits the specified Amazon Web Services
         * account or Amazon Web Services organization to put events to the specified
         * <i>event bus</i>. Amazon EventBridge (CloudWatch Events) rules in your account
         * are triggered by these events arriving to an event bus in your account. </p>
         * <p>For another account to send events to your account, that external account
         * must have an EventBridge rule with your account's event bus as a target.</p>
         * <p>To enable multiple Amazon Web Services accounts to put events to your event
         * bus, run <code>PutPermission</code> once for each of these accounts. Or, if all
         * the accounts are members of the same Amazon Web Services organization, you can
         * run <code>PutPermission</code> once specifying <code>Principal</code> as "*" and
         * specifying the Amazon Web Services organization ID in <code>Condition</code>, to
         * grant permissions to all accounts in that organization.</p> <p>If you grant
         * permissions using an organization, then accounts in that organization must
         * specify a <code>RoleArn</code> with proper permissions when they use
         * <code>PutTarget</code> to add your account's event bus as a target. For more
         * information, see <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/userguide/eventbridge-cross-account-event-delivery.html">Sending
         * and Receiving Events Between Amazon Web Services Accounts</a> in the <i>Amazon
         * EventBridge User Guide</i>.</p> <p>The permission policy on the event bus cannot
         * exceed 10 KB in size.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/PutPermission">AWS
         * API Reference</a></p>
         */
        virtual Model::PutPermissionOutcome PutPermission(const Model::PutPermissionRequest& request) const;


        /**
         * <p>Creates or updates the specified rule. Rules are enabled by default, or based
         * on value of the state. You can disable a rule using <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/APIReference/API_DisableRule.html">DisableRule</a>.</p>
         * <p>A single rule watches for events from a single event bus. Events generated by
         * Amazon Web Services services go to your account's default event bus. Events
         * generated by SaaS partner services or applications go to the matching partner
         * event bus. If you have custom applications or services, you can specify whether
         * their events go to your default event bus or a custom event bus that you have
         * created. For more information, see <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/APIReference/API_CreateEventBus.html">CreateEventBus</a>.</p>
         * <p>If you are updating an existing rule, the rule is replaced with what you
         * specify in this <code>PutRule</code> command. If you omit arguments in
         * <code>PutRule</code>, the old values for those arguments are not kept. Instead,
         * they are replaced with null values.</p> <p>When you create or update a rule,
         * incoming events might not immediately start matching to new or updated rules.
         * Allow a short period of time for changes to take effect.</p> <p>A rule must
         * contain at least an EventPattern or ScheduleExpression. Rules with EventPatterns
         * are triggered when a matching event is observed. Rules with ScheduleExpressions
         * self-trigger based on the given schedule. A rule can have both an EventPattern
         * and a ScheduleExpression, in which case the rule triggers on matching events as
         * well as on a schedule.</p> <p>When you initially create a rule, you can
         * optionally assign one or more tags to the rule. Tags can help you organize and
         * categorize your resources. You can also use them to scope user permissions, by
         * granting a user permission to access or change only rules with certain tag
         * values. To use the <code>PutRule</code> operation and assign tags, you must have
         * both the <code>events:PutRule</code> and <code>events:TagResource</code>
         * permissions.</p> <p>If you are updating an existing rule, any tags you specify
         * in the <code>PutRule</code> operation are ignored. To update the tags of an
         * existing rule, use <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/APIReference/API_TagResource.html">TagResource</a>
         * and <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/APIReference/API_UntagResource.html">UntagResource</a>.</p>
         * <p>Most services in Amazon Web Services treat : or / as the same character in
         * Amazon Resource Names (ARNs). However, EventBridge uses an exact match in event
         * patterns and rules. Be sure to use the correct ARN characters when creating
         * event patterns so that they match the ARN syntax in the event you want to
         * match.</p> <p>In EventBridge, it is possible to create rules that lead to
         * infinite loops, where a rule is fired repeatedly. For example, a rule might
         * detect that ACLs have changed on an S3 bucket, and trigger software to change
         * them to the desired state. If the rule is not written carefully, the subsequent
         * change to the ACLs fires the rule again, creating an infinite loop.</p> <p>To
         * prevent this, write the rules so that the triggered actions do not re-fire the
         * same rule. For example, your rule could fire only if ACLs are found to be in a
         * bad state, instead of after any change. </p> <p>An infinite loop can quickly
         * cause higher than expected charges. We recommend that you use budgeting, which
         * alerts you when charges exceed your specified limit. For more information, see
         * <a
         * href="https://docs.aws.amazon.com/awsaccountbilling/latest/aboutv2/budgets-managing-costs.html">Managing
         * Your Costs with Budgets</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/PutRule">AWS API
         * Reference</a></p>
         */
        virtual Model::PutRuleOutcome PutRule(const Model::PutRuleRequest& request) const;


        /**
         * <p>Adds the specified targets to the specified rule, or updates the targets if
         * they are already associated with the rule.</p> <p>Targets are the resources that
         * are invoked when a rule is triggered.</p> <p>You can configure the following as
         * targets for Events:</p> <ul> <li> <p> <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/userguide/eb-api-destinations.html">API
         * destination</a> </p> </li> <li> <p>Amazon API Gateway REST API endpoints</p>
         * </li> <li> <p>API Gateway</p> </li> <li> <p>Batch job queue</p> </li> <li>
         * <p>CloudWatch Logs group</p> </li> <li> <p>CodeBuild project</p> </li> <li>
         * <p>CodePipeline</p> </li> <li> <p>Amazon EC2 <code>CreateSnapshot</code> API
         * call</p> </li> <li> <p>Amazon EC2 <code>RebootInstances</code> API call</p>
         * </li> <li> <p>Amazon EC2 <code>StopInstances</code> API call</p> </li> <li>
         * <p>Amazon EC2 <code>TerminateInstances</code> API call</p> </li> <li> <p>Amazon
         * ECS tasks</p> </li> <li> <p>Event bus in a different Amazon Web Services account
         * or Region.</p> <p>You can use an event bus in the US East (N. Virginia)
         * us-east-1, US West (Oregon) us-west-2, or Europe (Ireland) eu-west-1 Regions as
         * a target for a rule.</p> </li> <li> <p>Firehose delivery stream (Kinesis Data
         * Firehose)</p> </li> <li> <p>Inspector assessment template (Amazon Inspector)</p>
         * </li> <li> <p>Kinesis stream (Kinesis Data Stream)</p> </li> <li> <p>Lambda
         * function</p> </li> <li> <p>Redshift clusters (Data API statement execution)</p>
         * </li> <li> <p>Amazon SNS topic</p> </li> <li> <p>Amazon SQS queues (includes
         * FIFO queues</p> </li> <li> <p>SSM Automation</p> </li> <li> <p>SSM OpsItem</p>
         * </li> <li> <p>SSM Run Command</p> </li> <li> <p>Step Functions state
         * machines</p> </li> </ul> <p>Creating rules with built-in targets is supported
         * only in the Management Console. The built-in targets are <code>EC2
         * CreateSnapshot API call</code>, <code>EC2 RebootInstances API call</code>,
         * <code>EC2 StopInstances API call</code>, and <code>EC2 TerminateInstances API
         * call</code>. </p> <p>For some target types, <code>PutTargets</code> provides
         * target-specific parameters. If the target is a Kinesis data stream, you can
         * optionally specify which shard the event goes to by using the
         * <code>KinesisParameters</code> argument. To invoke a command on multiple EC2
         * instances with one rule, you can use the <code>RunCommandParameters</code>
         * field.</p> <p>To be able to make API calls against the resources that you own,
         * Amazon EventBridge needs the appropriate permissions. For Lambda and Amazon SNS
         * resources, EventBridge relies on resource-based policies. For EC2 instances,
         * Kinesis Data Streams, Step Functions state machines and API Gateway REST APIs,
         * EventBridge relies on IAM roles that you specify in the <code>RoleARN</code>
         * argument in <code>PutTargets</code>. For more information, see <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/userguide/auth-and-access-control-eventbridge.html">Authentication
         * and Access Control</a> in the <i>Amazon EventBridge User Guide</i>.</p> <p>If
         * another Amazon Web Services account is in the same region and has granted you
         * permission (using <code>PutPermission</code>), you can send events to that
         * account. Set that account's event bus as a target of the rules in your account.
         * To send the matched events to the other account, specify that account's event
         * bus as the <code>Arn</code> value when you run <code>PutTargets</code>. If your
         * account sends events to another account, your account is charged for each sent
         * event. Each event sent to another account is charged as a custom event. The
         * account receiving the event is not charged. For more information, see <a
         * href="http://aws.amazon.com/eventbridge/pricing/">Amazon EventBridge
         * Pricing</a>.</p>  <p> <code>Input</code>, <code>InputPath</code>, and
         * <code>InputTransformer</code> are not available with <code>PutTarget</code> if
         * the target is an event bus of a different Amazon Web Services account.</p>
         *  <p>If you are setting the event bus of another account as the target,
         * and that account granted permission to your account through an organization
         * instead of directly by the account ID, then you must specify a
         * <code>RoleArn</code> with proper permissions in the <code>Target</code>
         * structure. For more information, see <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/userguide/eventbridge-cross-account-event-delivery.html">Sending
         * and Receiving Events Between Amazon Web Services Accounts</a> in the <i>Amazon
         * EventBridge User Guide</i>.</p> <p>For more information about enabling
         * cross-account events, see <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/APIReference/API_PutPermission.html">PutPermission</a>.</p>
         * <p> <b>Input</b>, <b>InputPath</b>, and <b>InputTransformer</b> are mutually
         * exclusive and optional parameters of a target. When a rule is triggered due to a
         * matched event:</p> <ul> <li> <p>If none of the following arguments are specified
         * for a target, then the entire event is passed to the target in JSON format
         * (unless the target is Amazon EC2 Run Command or Amazon ECS task, in which case
         * nothing from the event is passed to the target).</p> </li> <li> <p>If
         * <b>Input</b> is specified in the form of valid JSON, then the matched event is
         * overridden with this constant.</p> </li> <li> <p>If <b>InputPath</b> is
         * specified in the form of JSONPath (for example, <code>$.detail</code>), then
         * only the part of the event specified in the path is passed to the target (for
         * example, only the detail part of the event is passed).</p> </li> <li> <p>If
         * <b>InputTransformer</b> is specified, then one or more specified JSONPaths are
         * extracted from the event and used as values in a template that you specify as
         * the input to the target.</p> </li> </ul> <p>When you specify
         * <code>InputPath</code> or <code>InputTransformer</code>, you must use JSON dot
         * notation, not bracket notation.</p> <p>When you add targets to a rule and the
         * associated rule triggers soon after, new or updated targets might not be
         * immediately invoked. Allow a short period of time for changes to take
         * effect.</p> <p>This action can partially fail if too many requests are made at
         * the same time. If that happens, <code>FailedEntryCount</code> is non-zero in the
         * response and each entry in <code>FailedEntries</code> provides the ID of the
         * failed target and the error code.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/PutTargets">AWS
         * API Reference</a></p>
         */
        virtual Model::PutTargetsOutcome PutTargets(const Model::PutTargetsRequest& request) const;


        /**
         * <p>Revokes the permission of another Amazon Web Services account to be able to
         * put events to the specified event bus. Specify the account to revoke by the
         * <code>StatementId</code> value that you associated with the account when you
         * granted it permission with <code>PutPermission</code>. You can find the
         * <code>StatementId</code> by using <a
         * href="https://docs.aws.amazon.com/eventbridge/latest/APIReference/API_DescribeEventBus.html">DescribeEventBus</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/RemovePermission">AWS
         * API Reference</a></p>
         */
        virtual Model::RemovePermissionOutcome RemovePermission(const Model::RemovePermissionRequest& request) const;


        /**
         * <p>Removes the specified targets from the specified rule. When the rule is
         * triggered, those targets are no longer be invoked.</p> <p>When you remove a
         * target, when the associated rule triggers, removed targets might continue to be
         * invoked. Allow a short period of time for changes to take effect.</p> <p>This
         * action can partially fail if too many requests are made at the same time. If
         * that happens, <code>FailedEntryCount</code> is non-zero in the response and each
         * entry in <code>FailedEntries</code> provides the ID of the failed target and the
         * error code.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/RemoveTargets">AWS
         * API Reference</a></p>
         */
        virtual Model::RemoveTargetsOutcome RemoveTargets(const Model::RemoveTargetsRequest& request) const;


        /**
         * <p>Starts the specified replay. Events are not necessarily replayed in the exact
         * same order that they were added to the archive. A replay processes events to
         * replay based on the time in the event, and replays them using 1 minute
         * intervals. If you specify an <code>EventStartTime</code> and an
         * <code>EventEndTime</code> that covers a 20 minute time range, the events are
         * replayed from the first minute of that 20 minute range first. Then the events
         * from the second minute are replayed. You can use <code>DescribeReplay</code> to
         * determine the progress of a replay. The value returned for
         * <code>EventLastReplayedTime</code> indicates the time within the specified time
         * range associated with the last event replayed.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/StartReplay">AWS
         * API Reference</a></p>
         */
        virtual Model::StartReplayOutcome StartReplay(const Model::StartReplayRequest& request) const;


        /**
         * <p>Assigns one or more tags (key-value pairs) to the specified EventBridge
         * resource. Tags can help you organize and categorize your resources. You can also
         * use them to scope user permissions by granting a user permission to access or
         * change only resources with certain tag values. In EventBridge, rules and event
         * buses can be tagged.</p> <p>Tags don't have any semantic meaning to Amazon Web
         * Services and are interpreted strictly as strings of characters.</p> <p>You can
         * use the <code>TagResource</code> action with a resource that already has tags.
         * If you specify a new tag key, this tag is appended to the list of tags
         * associated with the resource. If you specify a tag key that is already
         * associated with the resource, the new tag value that you specify replaces the
         * previous value for that tag.</p> <p>You can associate as many as 50 tags with a
         * resource.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/TagResource">AWS
         * API Reference</a></p>
         */
        virtual Model::TagResourceOutcome TagResource(const Model::TagResourceRequest& request) const;


        /**
         * <p>Tests whether the specified event pattern matches the provided event.</p>
         * <p>Most services in Amazon Web Services treat : or / as the same character in
         * Amazon Resource Names (ARNs). However, EventBridge uses an exact match in event
         * patterns and rules. Be sure to use the correct ARN characters when creating
         * event patterns so that they match the ARN syntax in the event you want to
         * match.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/TestEventPattern">AWS
         * API Reference</a></p>
         */
        virtual Model::TestEventPatternOutcome TestEventPattern(const Model::TestEventPatternRequest& request) const;


        /**
         * <p>Removes one or more tags from the specified EventBridge resource. In Amazon
         * EventBridge (CloudWatch Events), rules and event buses can be
         * tagged.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/UntagResource">AWS
         * API Reference</a></p>
         */
        virtual Model::UntagResourceOutcome UntagResource(const Model::UntagResourceRequest& request) const;


        /**
         * <p>Updates an API destination.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/UpdateApiDestination">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateApiDestinationOutcome UpdateApiDestination(const Model::UpdateApiDestinationRequest& request) const;


        /**
         * <p>Updates the specified archive.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/UpdateArchive">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateArchiveOutcome UpdateArchive(const Model::UpdateArchiveRequest& request) const;


        /**
         * <p>Updates settings for a connection.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/events-2015-10-07/UpdateConnection">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateConnectionOutcome UpdateConnection(const Model::UpdateConnectionRequest& request) const;



      void OverrideEndpoint(const Aws::String& endpoint);
      std::shared_ptr<CloudWatchEventsEndpointProviderBase>& accessEndpointProvider();
    private:
      void init(const CloudWatchEventsClientConfiguration& clientConfiguration);

      CloudWatchEventsClientConfiguration m_clientConfiguration;
      std::shared_ptr<Aws::Utils::Threading::Executor> m_executor;
      std::shared_ptr<CloudWatchEventsEndpointProviderBase> m_endpointProvider;
  };

} // namespace CloudWatchEvents
} // namespace Aws
