﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/appconfig/AppConfig_EXPORTS.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/client/AWSClient.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/appconfig/AppConfigServiceClientModel.h>
#include <aws/appconfig/AppConfigLegacyAsyncMacros.h>

namespace Aws
{
namespace AppConfig
{
  /**
   * <p>Use AppConfig, a capability of Amazon Web Services Systems Manager, to
   * create, manage, and quickly deploy application configurations. AppConfig
   * supports controlled deployments to applications of any size and includes
   * built-in validation checks and monitoring. You can use AppConfig with
   * applications hosted on Amazon EC2 instances, Lambda, containers, mobile
   * applications, or IoT devices.</p> <p>To prevent errors when deploying
   * application configurations, especially for production systems where a simple
   * typo could cause an unexpected outage, AppConfig includes validators. A
   * validator provides a syntactic or semantic check to ensure that the
   * configuration you want to deploy works as intended. To validate your application
   * configuration data, you provide a schema or an Amazon Web Services Lambda
   * function that runs against the configuration. The configuration deployment or
   * update can only proceed when the configuration data is valid.</p> <p>During a
   * configuration deployment, AppConfig monitors the application to ensure that the
   * deployment is successful. If the system encounters an error, AppConfig rolls
   * back the change to minimize impact for your application users. You can configure
   * a deployment strategy for each application or environment that includes
   * deployment criteria, including velocity, bake time, and alarms to monitor.
   * Similar to error monitoring, if a deployment triggers an alarm, AppConfig
   * automatically rolls back to the previous version. </p> <p>AppConfig supports
   * multiple use cases. Here are some examples:</p> <ul> <li> <p> <b>Feature
   * flags</b>: Use AppConfig to turn on new features that require a timely
   * deployment, such as a product launch or announcement. </p> </li> <li> <p>
   * <b>Application tuning</b>: Use AppConfig to carefully introduce changes to your
   * application that can only be tested with production traffic.</p> </li> <li> <p>
   * <b>Allow list</b>: Use AppConfig to allow premium subscribers to access paid
   * content. </p> </li> <li> <p> <b>Operational issues</b>: Use AppConfig to reduce
   * stress on your application when a dependency or other external factor impacts
   * the system.</p> </li> </ul> <p>This reference is intended to be used with the <a
   * href="http://docs.aws.amazon.com/appconfig/latest/userguide/what-is-appconfig.html">AppConfig
   * User Guide</a>.</p>
   */
  class AWS_APPCONFIG_API AppConfigClient : public Aws::Client::AWSJsonClient
  {
    public:
      typedef Aws::Client::AWSJsonClient BASECLASS;
      static const char* SERVICE_NAME;
      static const char* ALLOCATION_TAG;

       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        AppConfigClient(const Aws::AppConfig::AppConfigClientConfiguration& clientConfiguration = Aws::AppConfig::AppConfigClientConfiguration(),
                        std::shared_ptr<AppConfigEndpointProviderBase> endpointProvider = Aws::MakeShared<AppConfigEndpointProvider>(ALLOCATION_TAG));

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        AppConfigClient(const Aws::Auth::AWSCredentials& credentials,
                        std::shared_ptr<AppConfigEndpointProviderBase> endpointProvider = Aws::MakeShared<AppConfigEndpointProvider>(ALLOCATION_TAG),
                        const Aws::AppConfig::AppConfigClientConfiguration& clientConfiguration = Aws::AppConfig::AppConfigClientConfiguration());

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        AppConfigClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                        std::shared_ptr<AppConfigEndpointProviderBase> endpointProvider = Aws::MakeShared<AppConfigEndpointProvider>(ALLOCATION_TAG),
                        const Aws::AppConfig::AppConfigClientConfiguration& clientConfiguration = Aws::AppConfig::AppConfigClientConfiguration());


        /* Legacy constructors due deprecation */
       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        AppConfigClient(const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        AppConfigClient(const Aws::Auth::AWSCredentials& credentials,
                        const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        AppConfigClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                        const Aws::Client::ClientConfiguration& clientConfiguration);

        /* End of legacy constructors due deprecation */
        virtual ~AppConfigClient();


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
         * <p>Creates an application. In AppConfig, an application is simply an
         * organizational construct like a folder. This organizational construct has a
         * relationship with some unit of executable code. For example, you could create an
         * application called MyMobileApp to organize and manage configuration data for a
         * mobile application installed by your users.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/CreateApplication">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateApplicationOutcome CreateApplication(const Model::CreateApplicationRequest& request) const;


        /**
         * <p>Creates a configuration profile, which is information that enables AppConfig
         * to access the configuration source. Valid configuration sources include the
         * AppConfig hosted configuration store, Amazon Web Services Systems Manager (SSM)
         * documents, SSM Parameter Store parameters, Amazon S3 objects, or any <a
         * href="http://docs.aws.amazon.com/codepipeline/latest/userguide/integrations-action-type.html#integrations-source">integration
         * source action</a> supported by CodePipeline. A configuration profile includes
         * the following information:</p> <ul> <li> <p>The URI location of the
         * configuration data.</p> </li> <li> <p>The Identity and Access Management (IAM)
         * role that provides access to the configuration data.</p> </li> <li> <p>A
         * validator for the configuration data. Available validators include either a JSON
         * Schema or an Amazon Web Services Lambda function.</p> </li> </ul> <p>For more
         * information, see <a
         * href="http://docs.aws.amazon.com/appconfig/latest/userguide/appconfig-creating-configuration-and-profile.html">Create
         * a Configuration and a Configuration Profile</a> in the <i>AppConfig User
         * Guide</i>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/CreateConfigurationProfile">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateConfigurationProfileOutcome CreateConfigurationProfile(const Model::CreateConfigurationProfileRequest& request) const;


        /**
         * <p>Creates a deployment strategy that defines important criteria for rolling out
         * your configuration to the designated targets. A deployment strategy includes the
         * overall duration required, a percentage of targets to receive the deployment
         * during each interval, an algorithm that defines how percentage grows, and bake
         * time.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/CreateDeploymentStrategy">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateDeploymentStrategyOutcome CreateDeploymentStrategy(const Model::CreateDeploymentStrategyRequest& request) const;


        /**
         * <p>Creates an environment. For each application, you define one or more
         * environments. An environment is a deployment group of AppConfig targets, such as
         * applications in a <code>Beta</code> or <code>Production</code> environment. You
         * can also define environments for application subcomponents such as the
         * <code>Web</code>, <code>Mobile</code> and <code>Back-end</code> components for
         * your application. You can configure Amazon CloudWatch alarms for each
         * environment. The system monitors alarms during a configuration deployment. If an
         * alarm is triggered, the system rolls back the configuration.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/CreateEnvironment">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateEnvironmentOutcome CreateEnvironment(const Model::CreateEnvironmentRequest& request) const;


        /**
         * <p>Creates an AppConfig extension. An extension augments your ability to inject
         * logic or behavior at different points during the AppConfig workflow of creating
         * or deploying a configuration.</p> <p>You can create your own extensions or use
         * the Amazon Web Services-authored extensions provided by AppConfig. For most
         * use-cases, to create your own extension, you must create an Lambda function to
         * perform any computation and processing defined in the extension. For more
         * information about extensions, see <a
         * href="https://docs.aws.amazon.com/appconfig/latest/userguide/working-with-appconfig-extensions.html">Working
         * with AppConfig extensions</a> in the <i>AppConfig User Guide</i>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/CreateExtension">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateExtensionOutcome CreateExtension(const Model::CreateExtensionRequest& request) const;


        /**
         * <p>When you create an extension or configure an Amazon Web Services-authored
         * extension, you associate the extension with an AppConfig application,
         * environment, or configuration profile. For example, you can choose to run the
         * <code>AppConfig deployment events to Amazon SNS</code> Amazon Web
         * Services-authored extension and receive notifications on an Amazon SNS topic
         * anytime a configuration deployment is started for a specific application.
         * Defining which extension to associate with an AppConfig resource is called an
         * <i>extension association</i>. An extension association is a specified
         * relationship between an extension and an AppConfig resource, such as an
         * application or a configuration profile. For more information about extensions
         * and associations, see <a
         * href="https://docs.aws.amazon.com/appconfig/latest/userguide/working-with-appconfig-extensions.html">Working
         * with AppConfig extensions</a> in the <i>AppConfig User Guide</i>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/CreateExtensionAssociation">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateExtensionAssociationOutcome CreateExtensionAssociation(const Model::CreateExtensionAssociationRequest& request) const;


        /**
         * <p>Creates a new configuration in the AppConfig hosted configuration
         * store.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/CreateHostedConfigurationVersion">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateHostedConfigurationVersionOutcome CreateHostedConfigurationVersion(const Model::CreateHostedConfigurationVersionRequest& request) const;


        /**
         * <p>Deletes an application. Deleting an application does not delete a
         * configuration from a host.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/DeleteApplication">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteApplicationOutcome DeleteApplication(const Model::DeleteApplicationRequest& request) const;


        /**
         * <p>Deletes a configuration profile. Deleting a configuration profile does not
         * delete a configuration from a host.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/DeleteConfigurationProfile">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteConfigurationProfileOutcome DeleteConfigurationProfile(const Model::DeleteConfigurationProfileRequest& request) const;


        /**
         * <p>Deletes a deployment strategy. Deleting a deployment strategy does not delete
         * a configuration from a host.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/DeleteDeploymentStrategy">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteDeploymentStrategyOutcome DeleteDeploymentStrategy(const Model::DeleteDeploymentStrategyRequest& request) const;


        /**
         * <p>Deletes an environment. Deleting an environment does not delete a
         * configuration from a host.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/DeleteEnvironment">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteEnvironmentOutcome DeleteEnvironment(const Model::DeleteEnvironmentRequest& request) const;


        /**
         * <p>Deletes an AppConfig extension. You must delete all associations to an
         * extension before you delete the extension.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/DeleteExtension">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteExtensionOutcome DeleteExtension(const Model::DeleteExtensionRequest& request) const;


        /**
         * <p>Deletes an extension association. This action doesn't delete extensions
         * defined in the association.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/DeleteExtensionAssociation">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteExtensionAssociationOutcome DeleteExtensionAssociation(const Model::DeleteExtensionAssociationRequest& request) const;


        /**
         * <p>Deletes a version of a configuration from the AppConfig hosted configuration
         * store.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/DeleteHostedConfigurationVersion">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteHostedConfigurationVersionOutcome DeleteHostedConfigurationVersion(const Model::DeleteHostedConfigurationVersionRequest& request) const;


        /**
         * <p>Retrieves information about an application.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/GetApplication">AWS
         * API Reference</a></p>
         */
        virtual Model::GetApplicationOutcome GetApplication(const Model::GetApplicationRequest& request) const;


        /**
         * <p>Retrieves information about a configuration profile.</p><p><h3>See Also:</h3>
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/GetConfigurationProfile">AWS
         * API Reference</a></p>
         */
        virtual Model::GetConfigurationProfileOutcome GetConfigurationProfile(const Model::GetConfigurationProfileRequest& request) const;


        /**
         * <p>Retrieves information about a configuration deployment.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/GetDeployment">AWS
         * API Reference</a></p>
         */
        virtual Model::GetDeploymentOutcome GetDeployment(const Model::GetDeploymentRequest& request) const;


        /**
         * <p>Retrieves information about a deployment strategy. A deployment strategy
         * defines important criteria for rolling out your configuration to the designated
         * targets. A deployment strategy includes the overall duration required, a
         * percentage of targets to receive the deployment during each interval, an
         * algorithm that defines how percentage grows, and bake time.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/GetDeploymentStrategy">AWS
         * API Reference</a></p>
         */
        virtual Model::GetDeploymentStrategyOutcome GetDeploymentStrategy(const Model::GetDeploymentStrategyRequest& request) const;


        /**
         * <p>Retrieves information about an environment. An environment is a deployment
         * group of AppConfig applications, such as applications in a
         * <code>Production</code> environment or in an <code>EU_Region</code> environment.
         * Each configuration deployment targets an environment. You can enable one or more
         * Amazon CloudWatch alarms for an environment. If an alarm is triggered during a
         * deployment, AppConfig roles back the configuration.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/GetEnvironment">AWS
         * API Reference</a></p>
         */
        virtual Model::GetEnvironmentOutcome GetEnvironment(const Model::GetEnvironmentRequest& request) const;


        /**
         * <p>Returns information about an AppConfig extension.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/GetExtension">AWS
         * API Reference</a></p>
         */
        virtual Model::GetExtensionOutcome GetExtension(const Model::GetExtensionRequest& request) const;


        /**
         * <p>Returns information about an AppConfig extension association. For more
         * information about extensions and associations, see <a
         * href="https://docs.aws.amazon.com/appconfig/latest/userguide/working-with-appconfig-extensions.html">Working
         * with AppConfig extensions</a> in the <i>AppConfig User Guide</i>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/GetExtensionAssociation">AWS
         * API Reference</a></p>
         */
        virtual Model::GetExtensionAssociationOutcome GetExtensionAssociation(const Model::GetExtensionAssociationRequest& request) const;


        /**
         * <p>Retrieves information about a specific configuration version.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/GetHostedConfigurationVersion">AWS
         * API Reference</a></p>
         */
        virtual Model::GetHostedConfigurationVersionOutcome GetHostedConfigurationVersion(const Model::GetHostedConfigurationVersionRequest& request) const;


        /**
         * <p>Lists all applications in your Amazon Web Services account.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/ListApplications">AWS
         * API Reference</a></p>
         */
        virtual Model::ListApplicationsOutcome ListApplications(const Model::ListApplicationsRequest& request) const;


        /**
         * <p>Lists the configuration profiles for an application.</p><p><h3>See Also:</h3>
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/ListConfigurationProfiles">AWS
         * API Reference</a></p>
         */
        virtual Model::ListConfigurationProfilesOutcome ListConfigurationProfiles(const Model::ListConfigurationProfilesRequest& request) const;


        /**
         * <p>Lists deployment strategies.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/ListDeploymentStrategies">AWS
         * API Reference</a></p>
         */
        virtual Model::ListDeploymentStrategiesOutcome ListDeploymentStrategies(const Model::ListDeploymentStrategiesRequest& request) const;


        /**
         * <p>Lists the deployments for an environment in descending deployment number
         * order.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/ListDeployments">AWS
         * API Reference</a></p>
         */
        virtual Model::ListDeploymentsOutcome ListDeployments(const Model::ListDeploymentsRequest& request) const;


        /**
         * <p>Lists the environments for an application.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/ListEnvironments">AWS
         * API Reference</a></p>
         */
        virtual Model::ListEnvironmentsOutcome ListEnvironments(const Model::ListEnvironmentsRequest& request) const;


        /**
         * <p>Lists all AppConfig extension associations in the account. For more
         * information about extensions and associations, see <a
         * href="https://docs.aws.amazon.com/appconfig/latest/userguide/working-with-appconfig-extensions.html">Working
         * with AppConfig extensions</a> in the <i>AppConfig User Guide</i>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/ListExtensionAssociations">AWS
         * API Reference</a></p>
         */
        virtual Model::ListExtensionAssociationsOutcome ListExtensionAssociations(const Model::ListExtensionAssociationsRequest& request) const;


        /**
         * <p>Lists all custom and Amazon Web Services-authored AppConfig extensions in the
         * account. For more information about extensions, see <a
         * href="https://docs.aws.amazon.com/appconfig/latest/userguide/working-with-appconfig-extensions.html">Working
         * with AppConfig extensions</a> in the <i>AppConfig User Guide</i>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/ListExtensions">AWS
         * API Reference</a></p>
         */
        virtual Model::ListExtensionsOutcome ListExtensions(const Model::ListExtensionsRequest& request) const;


        /**
         * <p>Lists configurations stored in the AppConfig hosted configuration store by
         * version.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/ListHostedConfigurationVersions">AWS
         * API Reference</a></p>
         */
        virtual Model::ListHostedConfigurationVersionsOutcome ListHostedConfigurationVersions(const Model::ListHostedConfigurationVersionsRequest& request) const;


        /**
         * <p>Retrieves the list of key-value tags assigned to the resource.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/ListTagsForResource">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTagsForResourceOutcome ListTagsForResource(const Model::ListTagsForResourceRequest& request) const;


        /**
         * <p>Starts a deployment.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/StartDeployment">AWS
         * API Reference</a></p>
         */
        virtual Model::StartDeploymentOutcome StartDeployment(const Model::StartDeploymentRequest& request) const;


        /**
         * <p>Stops a deployment. This API action works only on deployments that have a
         * status of <code>DEPLOYING</code>. This action moves the deployment to a status
         * of <code>ROLLED_BACK</code>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/StopDeployment">AWS
         * API Reference</a></p>
         */
        virtual Model::StopDeploymentOutcome StopDeployment(const Model::StopDeploymentRequest& request) const;


        /**
         * <p>Assigns metadata to an AppConfig resource. Tags help organize and categorize
         * your AppConfig resources. Each tag consists of a key and an optional value, both
         * of which you define. You can specify a maximum of 50 tags for a
         * resource.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/TagResource">AWS
         * API Reference</a></p>
         */
        virtual Model::TagResourceOutcome TagResource(const Model::TagResourceRequest& request) const;


        /**
         * <p>Deletes a tag key and value from an AppConfig resource.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/UntagResource">AWS
         * API Reference</a></p>
         */
        virtual Model::UntagResourceOutcome UntagResource(const Model::UntagResourceRequest& request) const;


        /**
         * <p>Updates an application.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/UpdateApplication">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateApplicationOutcome UpdateApplication(const Model::UpdateApplicationRequest& request) const;


        /**
         * <p>Updates a configuration profile.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/UpdateConfigurationProfile">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateConfigurationProfileOutcome UpdateConfigurationProfile(const Model::UpdateConfigurationProfileRequest& request) const;


        /**
         * <p>Updates a deployment strategy.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/UpdateDeploymentStrategy">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateDeploymentStrategyOutcome UpdateDeploymentStrategy(const Model::UpdateDeploymentStrategyRequest& request) const;


        /**
         * <p>Updates an environment.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/UpdateEnvironment">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateEnvironmentOutcome UpdateEnvironment(const Model::UpdateEnvironmentRequest& request) const;


        /**
         * <p>Updates an AppConfig extension. For more information about extensions, see <a
         * href="https://docs.aws.amazon.com/appconfig/latest/userguide/working-with-appconfig-extensions.html">Working
         * with AppConfig extensions</a> in the <i>AppConfig User Guide</i>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/UpdateExtension">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateExtensionOutcome UpdateExtension(const Model::UpdateExtensionRequest& request) const;


        /**
         * <p>Updates an association. For more information about extensions and
         * associations, see <a
         * href="https://docs.aws.amazon.com/appconfig/latest/userguide/working-with-appconfig-extensions.html">Working
         * with AppConfig extensions</a> in the <i>AppConfig User Guide</i>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/UpdateExtensionAssociation">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateExtensionAssociationOutcome UpdateExtensionAssociation(const Model::UpdateExtensionAssociationRequest& request) const;


        /**
         * <p>Uses the validators in a configuration profile to validate a
         * configuration.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/appconfig-2019-10-09/ValidateConfiguration">AWS
         * API Reference</a></p>
         */
        virtual Model::ValidateConfigurationOutcome ValidateConfiguration(const Model::ValidateConfigurationRequest& request) const;



      void OverrideEndpoint(const Aws::String& endpoint);
      std::shared_ptr<AppConfigEndpointProviderBase>& accessEndpointProvider();
    private:
      void init(const AppConfigClientConfiguration& clientConfiguration);

      AppConfigClientConfiguration m_clientConfiguration;
      std::shared_ptr<Aws::Utils::Threading::Executor> m_executor;
      std::shared_ptr<AppConfigEndpointProviderBase> m_endpointProvider;
  };

} // namespace AppConfig
} // namespace Aws
