﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/evidently/CloudWatchEvidently_EXPORTS.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/client/AWSClient.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/evidently/CloudWatchEvidentlyServiceClientModel.h>
#include <aws/evidently/CloudWatchEvidentlyLegacyAsyncMacros.h>

namespace Aws
{
namespace CloudWatchEvidently
{
  /**
   * <p>You can use Amazon CloudWatch Evidently to safely validate new features by
   * serving them to a specified percentage of your users while you roll out the
   * feature. You can monitor the performance of the new feature to help you decide
   * when to ramp up traffic to your users. This helps you reduce risk and identify
   * unintended consequences before you fully launch the feature.</p> <p>You can also
   * conduct A/B experiments to make feature design decisions based on evidence and
   * data. An experiment can test as many as five variations at once. Evidently
   * collects experiment data and analyzes it using statistical methods. It also
   * provides clear recommendations about which variations perform better. You can
   * test both user-facing features and backend features.</p>
   */
  class AWS_CLOUDWATCHEVIDENTLY_API CloudWatchEvidentlyClient : public Aws::Client::AWSJsonClient
  {
    public:
      typedef Aws::Client::AWSJsonClient BASECLASS;
      static const char* SERVICE_NAME;
      static const char* ALLOCATION_TAG;

       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CloudWatchEvidentlyClient(const Aws::CloudWatchEvidently::CloudWatchEvidentlyClientConfiguration& clientConfiguration = Aws::CloudWatchEvidently::CloudWatchEvidentlyClientConfiguration(),
                                  std::shared_ptr<CloudWatchEvidentlyEndpointProviderBase> endpointProvider = Aws::MakeShared<CloudWatchEvidentlyEndpointProvider>(ALLOCATION_TAG));

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CloudWatchEvidentlyClient(const Aws::Auth::AWSCredentials& credentials,
                                  std::shared_ptr<CloudWatchEvidentlyEndpointProviderBase> endpointProvider = Aws::MakeShared<CloudWatchEvidentlyEndpointProvider>(ALLOCATION_TAG),
                                  const Aws::CloudWatchEvidently::CloudWatchEvidentlyClientConfiguration& clientConfiguration = Aws::CloudWatchEvidently::CloudWatchEvidentlyClientConfiguration());

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        CloudWatchEvidentlyClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                                  std::shared_ptr<CloudWatchEvidentlyEndpointProviderBase> endpointProvider = Aws::MakeShared<CloudWatchEvidentlyEndpointProvider>(ALLOCATION_TAG),
                                  const Aws::CloudWatchEvidently::CloudWatchEvidentlyClientConfiguration& clientConfiguration = Aws::CloudWatchEvidently::CloudWatchEvidentlyClientConfiguration());


        /* Legacy constructors due deprecation */
       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CloudWatchEvidentlyClient(const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CloudWatchEvidentlyClient(const Aws::Auth::AWSCredentials& credentials,
                                  const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        CloudWatchEvidentlyClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                                  const Aws::Client::ClientConfiguration& clientConfiguration);

        /* End of legacy constructors due deprecation */
        virtual ~CloudWatchEvidentlyClient();


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
         * <p>This operation assigns feature variation to user sessions. For each user
         * session, you pass in an <code>entityID</code> that represents the user.
         * Evidently then checks the evaluation rules and assigns the variation.</p> <p>The
         * first rules that are evaluated are the override rules. If the user's
         * <code>entityID</code> matches an override rule, the user is served the variation
         * specified by that rule.</p> <p>Next, if there is a launch of the feature, the
         * user might be assigned to a variation in the launch. The chance of this depends
         * on the percentage of users that are allocated to that launch. If the user is
         * enrolled in the launch, the variation they are served depends on the allocation
         * of the various feature variations used for the launch.</p> <p>If the user is not
         * assigned to a launch, and there is an ongoing experiment for this feature, the
         * user might be assigned to a variation in the experiment. The chance of this
         * depends on the percentage of users that are allocated to that experiment. If the
         * user is enrolled in the experiment, the variation they are served depends on the
         * allocation of the various feature variations used for the experiment. </p> <p>If
         * the user is not assigned to a launch or experiment, they are served the default
         * variation.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/BatchEvaluateFeature">AWS
         * API Reference</a></p>
         */
        virtual Model::BatchEvaluateFeatureOutcome BatchEvaluateFeature(const Model::BatchEvaluateFeatureRequest& request) const;


        /**
         * <p>Creates an Evidently <i>experiment</i>. Before you create an experiment, you
         * must create the feature to use for the experiment.</p> <p>An experiment helps
         * you make feature design decisions based on evidence and data. An experiment can
         * test as many as five variations at once. Evidently collects experiment data and
         * analyzes it by statistical methods, and provides clear recommendations about
         * which variations perform better.</p> <p>You can optionally specify a
         * <code>segment</code> to have the experiment consider only certain audience types
         * in the experiment, such as using only user sessions from a certain location or
         * who use a certain internet browser.</p> <p>Don't use this operation to update an
         * existing experiment. Instead, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_UpdateExperiment.html">UpdateExperiment</a>.
         * </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/CreateExperiment">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateExperimentOutcome CreateExperiment(const Model::CreateExperimentRequest& request) const;


        /**
         * <p>Creates an Evidently <i>feature</i> that you want to launch or test. You can
         * define up to five variations of a feature, and use these variations in your
         * launches and experiments. A feature must be created in a project. For
         * information about creating a project, see <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_CreateProject.html">CreateProject</a>.</p>
         * <p>Don't use this operation to update an existing feature. Instead, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_UpdateFeature.html">UpdateFeature</a>.
         * </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/CreateFeature">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateFeatureOutcome CreateFeature(const Model::CreateFeatureRequest& request) const;


        /**
         * <p>Creates a <i>launch</i> of a given feature. Before you create a launch, you
         * must create the feature to use for the launch.</p> <p>You can use a launch to
         * safely validate new features by serving them to a specified percentage of your
         * users while you roll out the feature. You can monitor the performance of the new
         * feature to help you decide when to ramp up traffic to more users. This helps you
         * reduce risk and identify unintended consequences before you fully launch the
         * feature.</p> <p>Don't use this operation to update an existing launch. Instead,
         * use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_UpdateLaunch.html">UpdateLaunch</a>.
         * </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/CreateLaunch">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateLaunchOutcome CreateLaunch(const Model::CreateLaunchRequest& request) const;


        /**
         * <p>Creates a project, which is the logical object in Evidently that can contain
         * features, launches, and experiments. Use projects to group similar features
         * together.</p> <p>To update an existing project, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_UpdateProject.html">UpdateProject</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/CreateProject">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateProjectOutcome CreateProject(const Model::CreateProjectRequest& request) const;


        /**
         * <p>Use this operation to define a <i>segment</i> of your audience. A segment is
         * a portion of your audience that share one or more characteristics. Examples
         * could be Chrome browser users, users in Europe, or Firefox browser users in
         * Europe who also fit other criteria that your application collects, such as
         * age.</p> <p>Using a segment in an experiment limits that experiment to evaluate
         * only the users who match the segment criteria. Using one or more segments in a
         * launch allows you to define different traffic splits for the different audience
         * segments.</p> <pre><code> &lt;p&gt;For more information about segment pattern
         * syntax, see &lt;a
         * href=&quot;https://docs.aws.amazon.com/AmazonCloudWatch/latest/monitoring/CloudWatch-Evidently-segments.html#CloudWatch-Evidently-segments-syntax.html&quot;&gt;
         * Segment rule pattern syntax&lt;/a&gt;.&lt;/p&gt; &lt;p&gt;The pattern that you
         * define for a segment is matched against the value of
         * &lt;code&gt;evaluationContext&lt;/code&gt;, which is passed into Evidently in
         * the &lt;a
         * href=&quot;https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_EvaluateFeature.html&quot;&gt;EvaluateFeature&lt;/a&gt;
         * operation, when Evidently assigns a feature variation to a user.&lt;/p&gt;
         * </code></pre><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/CreateSegment">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateSegmentOutcome CreateSegment(const Model::CreateSegmentRequest& request) const;


        /**
         * <p>Deletes an Evidently experiment. The feature used for the experiment is not
         * deleted.</p> <p>To stop an experiment without deleting it, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_StopExperiment.html">StopExperiment</a>.
         * </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/DeleteExperiment">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteExperimentOutcome DeleteExperiment(const Model::DeleteExperimentRequest& request) const;


        /**
         * <p>Deletes an Evidently feature.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/DeleteFeature">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteFeatureOutcome DeleteFeature(const Model::DeleteFeatureRequest& request) const;


        /**
         * <p>Deletes an Evidently launch. The feature used for the launch is not
         * deleted.</p> <p>To stop a launch without deleting it, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_StopLaunch.html">StopLaunch</a>.
         * </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/DeleteLaunch">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteLaunchOutcome DeleteLaunch(const Model::DeleteLaunchRequest& request) const;


        /**
         * <p>Deletes an Evidently project. Before you can delete a project, you must
         * delete all the features that the project contains. To delete a feature, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_DeleteFeature.html">DeleteFeature</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/DeleteProject">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteProjectOutcome DeleteProject(const Model::DeleteProjectRequest& request) const;


        /**
         * <p>Deletes a segment. You can't delete a segment that is being used in a launch
         * or experiment, even if that launch or experiment is not currently
         * running.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/DeleteSegment">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteSegmentOutcome DeleteSegment(const Model::DeleteSegmentRequest& request) const;


        /**
         * <p>This operation assigns a feature variation to one given user session. You
         * pass in an <code>entityID</code> that represents the user. Evidently then checks
         * the evaluation rules and assigns the variation.</p> <p>The first rules that are
         * evaluated are the override rules. If the user's <code>entityID</code> matches an
         * override rule, the user is served the variation specified by that rule.</p>
         * <pre><code> &lt;p&gt;If there is a current launch with this feature that uses
         * segment overrides, and if the user session's
         * &lt;code&gt;evaluationContext&lt;/code&gt; matches a segment rule defined in a
         * segment override, the configuration in the segment overrides is used. For more
         * information about segments, see &lt;a
         * href=&quot;https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_CreateSegment.html&quot;&gt;CreateSegment&lt;/a&gt;
         * and &lt;a
         * href=&quot;https://docs.aws.amazon.com/AmazonCloudWatch/latest/monitoring/CloudWatch-Evidently-segments.html&quot;&gt;Use
         * segments to focus your audience&lt;/a&gt;.&lt;/p&gt; &lt;p&gt;If there is a
         * launch with no segment overrides, the user might be assigned to a variation in
         * the launch. The chance of this depends on the percentage of users that are
         * allocated to that launch. If the user is enrolled in the launch, the variation
         * they are served depends on the allocation of the various feature variations used
         * for the launch.&lt;/p&gt; &lt;p&gt;If the user is not assigned to a launch, and
         * there is an ongoing experiment for this feature, the user might be assigned to a
         * variation in the experiment. The chance of this depends on the percentage of
         * users that are allocated to that experiment.&lt;/p&gt; &lt;p&gt;If the
         * experiment uses a segment, then only user sessions with
         * &lt;code&gt;evaluationContext&lt;/code&gt; values that match the segment rule
         * are used in the experiment.&lt;/p&gt; &lt;p&gt;If the user is enrolled in the
         * experiment, the variation they are served depends on the allocation of the
         * various feature variations used for the experiment. &lt;/p&gt; &lt;p&gt;If the
         * user is not assigned to a launch or experiment, they are served the default
         * variation.&lt;/p&gt; </code></pre><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/EvaluateFeature">AWS
         * API Reference</a></p>
         */
        virtual Model::EvaluateFeatureOutcome EvaluateFeature(const Model::EvaluateFeatureRequest& request) const;


        /**
         * <p>Returns the details about one experiment. You must already know the
         * experiment name. To retrieve a list of experiments in your account, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_ListExperiments.html">ListExperiments</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/GetExperiment">AWS
         * API Reference</a></p>
         */
        virtual Model::GetExperimentOutcome GetExperiment(const Model::GetExperimentRequest& request) const;


        /**
         * <p>Retrieves the results of a running or completed experiment. No results are
         * available until there have been 100 events for each variation and at least 10
         * minutes have passed since the start of the experiment. To increase the
         * statistical power, Evidently performs an additional offline p-value analysis at
         * the end of the experiment. Offline p-value analysis can detect statistical
         * significance in some cases where the anytime p-values used during the experiment
         * do not find statistical significance.</p> <p>Experiment results are available up
         * to 63 days after the start of the experiment. They are not available after that
         * because of CloudWatch data retention policies.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/GetExperimentResults">AWS
         * API Reference</a></p>
         */
        virtual Model::GetExperimentResultsOutcome GetExperimentResults(const Model::GetExperimentResultsRequest& request) const;


        /**
         * <p>Returns the details about one feature. You must already know the feature
         * name. To retrieve a list of features in your account, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_ListFeatures.html">ListFeatures</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/GetFeature">AWS
         * API Reference</a></p>
         */
        virtual Model::GetFeatureOutcome GetFeature(const Model::GetFeatureRequest& request) const;


        /**
         * <p>Returns the details about one launch. You must already know the launch name.
         * To retrieve a list of launches in your account, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_ListLaunches.html">ListLaunches</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/GetLaunch">AWS
         * API Reference</a></p>
         */
        virtual Model::GetLaunchOutcome GetLaunch(const Model::GetLaunchRequest& request) const;


        /**
         * <p>Returns the details about one launch. You must already know the project name.
         * To retrieve a list of projects in your account, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_ListProjects.html">ListProjects</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/GetProject">AWS
         * API Reference</a></p>
         */
        virtual Model::GetProjectOutcome GetProject(const Model::GetProjectRequest& request) const;


        /**
         * <p>Returns information about the specified segment. Specify the segment you want
         * to view by specifying its ARN.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/GetSegment">AWS
         * API Reference</a></p>
         */
        virtual Model::GetSegmentOutcome GetSegment(const Model::GetSegmentRequest& request) const;


        /**
         * <p>Returns configuration details about all the experiments in the specified
         * project.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/ListExperiments">AWS
         * API Reference</a></p>
         */
        virtual Model::ListExperimentsOutcome ListExperiments(const Model::ListExperimentsRequest& request) const;


        /**
         * <p>Returns configuration details about all the features in the specified
         * project.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/ListFeatures">AWS
         * API Reference</a></p>
         */
        virtual Model::ListFeaturesOutcome ListFeatures(const Model::ListFeaturesRequest& request) const;


        /**
         * <p>Returns configuration details about all the launches in the specified
         * project.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/ListLaunches">AWS
         * API Reference</a></p>
         */
        virtual Model::ListLaunchesOutcome ListLaunches(const Model::ListLaunchesRequest& request) const;


        /**
         * <p>Returns configuration details about all the projects in the current Region in
         * your account.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/ListProjects">AWS
         * API Reference</a></p>
         */
        virtual Model::ListProjectsOutcome ListProjects(const Model::ListProjectsRequest& request) const;


        /**
         * <p>Use this operation to find which experiments or launches are using a
         * specified segment.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/ListSegmentReferences">AWS
         * API Reference</a></p>
         */
        virtual Model::ListSegmentReferencesOutcome ListSegmentReferences(const Model::ListSegmentReferencesRequest& request) const;


        /**
         * <p>Returns a list of audience segments that you have created in your account in
         * this Region.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/ListSegments">AWS
         * API Reference</a></p>
         */
        virtual Model::ListSegmentsOutcome ListSegments(const Model::ListSegmentsRequest& request) const;


        /**
         * <p>Displays the tags associated with an Evidently resource.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/ListTagsForResource">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTagsForResourceOutcome ListTagsForResource(const Model::ListTagsForResourceRequest& request) const;


        /**
         * <p>Sends performance events to Evidently. These events can be used to evaluate a
         * launch or an experiment.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/PutProjectEvents">AWS
         * API Reference</a></p>
         */
        virtual Model::PutProjectEventsOutcome PutProjectEvents(const Model::PutProjectEventsRequest& request) const;


        /**
         * <p>Starts an existing experiment. To create an experiment, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_CreateExperiment.html">CreateExperiment</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/StartExperiment">AWS
         * API Reference</a></p>
         */
        virtual Model::StartExperimentOutcome StartExperiment(const Model::StartExperimentRequest& request) const;


        /**
         * <p>Starts an existing launch. To create a launch, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_CreateLaunch.html">CreateLaunch</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/StartLaunch">AWS
         * API Reference</a></p>
         */
        virtual Model::StartLaunchOutcome StartLaunch(const Model::StartLaunchRequest& request) const;


        /**
         * <p>Stops an experiment that is currently running. If you stop an experiment, you
         * can't resume it or restart it.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/StopExperiment">AWS
         * API Reference</a></p>
         */
        virtual Model::StopExperimentOutcome StopExperiment(const Model::StopExperimentRequest& request) const;


        /**
         * <p>Stops a launch that is currently running. After you stop a launch, you will
         * not be able to resume it or restart it. Also, it will not be evaluated as a rule
         * for traffic allocation, and the traffic that was allocated to the launch will
         * instead be available to the feature's experiment, if there is one. Otherwise,
         * all traffic will be served the default variation after the launch is
         * stopped.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/StopLaunch">AWS
         * API Reference</a></p>
         */
        virtual Model::StopLaunchOutcome StopLaunch(const Model::StopLaunchRequest& request) const;


        /**
         * <p>Assigns one or more tags (key-value pairs) to the specified CloudWatch
         * Evidently resource. Projects, features, launches, and experiments can be
         * tagged.</p> <p>Tags can help you organize and categorize your resources. You can
         * also use them to scope user permissions by granting a user permission to access
         * or change only resources with certain tag values.</p> <p>Tags don't have any
         * semantic meaning to Amazon Web Services and are interpreted strictly as strings
         * of characters.</p> <p>You can use the <code>TagResource</code> action with a
         * resource that already has tags. If you specify a new tag key for the resource,
         * this tag is appended to the list of tags associated with the alarm. If you
         * specify a tag key that is already associated with the resource, the new tag
         * value that you specify replaces the previous value for that tag.</p> <p>You can
         * associate as many as 50 tags with a resource.</p> <p>For more information, see
         * <a href="https://docs.aws.amazon.com/general/latest/gr/aws_tagging.html">Tagging
         * Amazon Web Services resources</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/TagResource">AWS
         * API Reference</a></p>
         */
        virtual Model::TagResourceOutcome TagResource(const Model::TagResourceRequest& request) const;


        /**
         * <p>Use this operation to test a rules pattern that you plan to use to create an
         * audience segment. For more information about segments, see <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_CreateSegment.html">CreateSegment</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/TestSegmentPattern">AWS
         * API Reference</a></p>
         */
        virtual Model::TestSegmentPatternOutcome TestSegmentPattern(const Model::TestSegmentPatternRequest& request) const;


        /**
         * <p>Removes one or more tags from the specified resource.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/UntagResource">AWS
         * API Reference</a></p>
         */
        virtual Model::UntagResourceOutcome UntagResource(const Model::UntagResourceRequest& request) const;


        /**
         * <p>Updates an Evidently experiment. </p> <p>Don't use this operation to update
         * an experiment's tag. Instead, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_TagResource.html">TagResource</a>.
         * </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/UpdateExperiment">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateExperimentOutcome UpdateExperiment(const Model::UpdateExperimentRequest& request) const;


        /**
         * <p>Updates an existing feature.</p> <p>You can't use this operation to update
         * the tags of an existing feature. Instead, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_TagResource.html">TagResource</a>.
         * </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/UpdateFeature">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateFeatureOutcome UpdateFeature(const Model::UpdateFeatureRequest& request) const;


        /**
         * <p>Updates a launch of a given feature. </p> <p>Don't use this operation to
         * update the tags of an existing launch. Instead, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_TagResource.html">TagResource</a>.
         * </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/UpdateLaunch">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateLaunchOutcome UpdateLaunch(const Model::UpdateLaunchRequest& request) const;


        /**
         * <p>Updates the description of an existing project.</p> <p>To create a new
         * project, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_CreateProject.html">CreateProject</a>.</p>
         * <p>Don't use this operation to update the data storage options of a project.
         * Instead, use <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_UpdateProjectDataDelivery.html">UpdateProjectDataDelivery</a>.
         * </p> <p>Don't use this operation to update the tags of a project. Instead, use
         * <a
         * href="https://docs.aws.amazon.com/cloudwatchevidently/latest/APIReference/API_TagResource.html">TagResource</a>.
         * </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/UpdateProject">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateProjectOutcome UpdateProject(const Model::UpdateProjectRequest& request) const;


        /**
         * <p>Updates the data storage options for this project. If you store evaluation
         * events, you an keep them and analyze them on your own. If you choose not to
         * store evaluation events, Evidently deletes them after using them to produce
         * metrics and other experiment results that you can view.</p> <p>You can't specify
         * both <code>cloudWatchLogs</code> and <code>s3Destination</code> in the same
         * operation.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/evidently-2021-02-01/UpdateProjectDataDelivery">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateProjectDataDeliveryOutcome UpdateProjectDataDelivery(const Model::UpdateProjectDataDeliveryRequest& request) const;



      void OverrideEndpoint(const Aws::String& endpoint);
      std::shared_ptr<CloudWatchEvidentlyEndpointProviderBase>& accessEndpointProvider();
    private:
      void init(const CloudWatchEvidentlyClientConfiguration& clientConfiguration);

      CloudWatchEvidentlyClientConfiguration m_clientConfiguration;
      std::shared_ptr<Aws::Utils::Threading::Executor> m_executor;
      std::shared_ptr<CloudWatchEvidentlyEndpointProviderBase> m_endpointProvider;
  };

} // namespace CloudWatchEvidently
} // namespace Aws
