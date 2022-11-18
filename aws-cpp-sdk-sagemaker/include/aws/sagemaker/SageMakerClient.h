﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/sagemaker/SageMaker_EXPORTS.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/client/AWSClient.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/sagemaker/SageMakerServiceClientModel.h>
#include <aws/sagemaker/SageMakerLegacyAsyncMacros.h>

namespace Aws
{
namespace SageMaker
{
  /**
   * <p>Provides APIs for creating and managing SageMaker resources. </p> <p>Other
   * Resources:</p> <ul> <li> <p> <a
   * href="https://docs.aws.amazon.com/sagemaker/latest/dg/whatis.html#first-time-user">SageMaker
   * Developer Guide</a> </p> </li> <li> <p> <a
   * href="https://docs.aws.amazon.com/augmented-ai/2019-11-07/APIReference/Welcome.html">Amazon
   * Augmented AI Runtime API Reference</a> </p> </li> </ul>
   */
  class AWS_SAGEMAKER_API SageMakerClient : public Aws::Client::AWSJsonClient
  {
    public:
      typedef Aws::Client::AWSJsonClient BASECLASS;
      static const char* SERVICE_NAME;
      static const char* ALLOCATION_TAG;

       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        SageMakerClient(const Aws::SageMaker::SageMakerClientConfiguration& clientConfiguration = Aws::SageMaker::SageMakerClientConfiguration(),
                        std::shared_ptr<SageMakerEndpointProviderBase> endpointProvider = Aws::MakeShared<SageMakerEndpointProvider>(ALLOCATION_TAG));

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        SageMakerClient(const Aws::Auth::AWSCredentials& credentials,
                        std::shared_ptr<SageMakerEndpointProviderBase> endpointProvider = Aws::MakeShared<SageMakerEndpointProvider>(ALLOCATION_TAG),
                        const Aws::SageMaker::SageMakerClientConfiguration& clientConfiguration = Aws::SageMaker::SageMakerClientConfiguration());

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        SageMakerClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                        std::shared_ptr<SageMakerEndpointProviderBase> endpointProvider = Aws::MakeShared<SageMakerEndpointProvider>(ALLOCATION_TAG),
                        const Aws::SageMaker::SageMakerClientConfiguration& clientConfiguration = Aws::SageMaker::SageMakerClientConfiguration());


        /* Legacy constructors due deprecation */
       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        SageMakerClient(const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        SageMakerClient(const Aws::Auth::AWSCredentials& credentials,
                        const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        SageMakerClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                        const Aws::Client::ClientConfiguration& clientConfiguration);

        /* End of legacy constructors due deprecation */
        virtual ~SageMakerClient();


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
         * <p>Creates an <i>association</i> between the source and the destination. A
         * source can be associated with multiple destinations, and a destination can be
         * associated with multiple sources. An association is a lineage tracking entity.
         * For more information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/lineage-tracking.html">Amazon
         * SageMaker ML Lineage Tracking</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/AddAssociation">AWS
         * API Reference</a></p>
         */
        virtual Model::AddAssociationOutcome AddAssociation(const Model::AddAssociationRequest& request) const;


        /**
         * <p>Adds or overwrites one or more tags for the specified SageMaker resource. You
         * can add tags to notebook instances, training jobs, hyperparameter tuning jobs,
         * batch transform jobs, models, labeling jobs, work teams, endpoint
         * configurations, and endpoints.</p> <p>Each tag consists of a key and an optional
         * value. Tag keys must be unique per resource. For more information about tags,
         * see For more information, see <a
         * href="https://aws.amazon.com/answers/account-management/aws-tagging-strategies/">Amazon
         * Web Services Tagging Strategies</a>.</p>  <p>Tags that you add to a
         * hyperparameter tuning job by calling this API are also added to any training
         * jobs that the hyperparameter tuning job launches after you call this API, but
         * not to training jobs that the hyperparameter tuning job launched before you
         * called this API. To make sure that the tags associated with a hyperparameter
         * tuning job are also added to all training jobs that the hyperparameter tuning
         * job launches, add the tags when you first create the tuning job by specifying
         * them in the <code>Tags</code> parameter of <a>CreateHyperParameterTuningJob</a>
         * </p>   <p>Tags that you add to a SageMaker Studio Domain or User
         * Profile by calling this API are also added to any Apps that the Domain or User
         * Profile launches after you call this API, but not to Apps that the Domain or
         * User Profile launched before you called this API. To make sure that the tags
         * associated with a Domain or User Profile are also added to all Apps that the
         * Domain or User Profile launches, add the tags when you first create the Domain
         * or User Profile by specifying them in the <code>Tags</code> parameter of
         * <a>CreateDomain</a> or <a>CreateUserProfile</a>.</p> <p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/AddTags">AWS
         * API Reference</a></p>
         */
        virtual Model::AddTagsOutcome AddTags(const Model::AddTagsRequest& request) const;


        /**
         * <p>Associates a trial component with a trial. A trial component can be
         * associated with multiple trials. To disassociate a trial component from a trial,
         * call the <a>DisassociateTrialComponent</a> API.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/AssociateTrialComponent">AWS
         * API Reference</a></p>
         */
        virtual Model::AssociateTrialComponentOutcome AssociateTrialComponent(const Model::AssociateTrialComponentRequest& request) const;


        /**
         * <p>This action batch describes a list of versioned model packages</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/BatchDescribeModelPackage">AWS
         * API Reference</a></p>
         */
        virtual Model::BatchDescribeModelPackageOutcome BatchDescribeModelPackage(const Model::BatchDescribeModelPackageRequest& request) const;


        /**
         * <p>Creates an <i>action</i>. An action is a lineage tracking entity that
         * represents an action or activity. For example, a model deployment or an HPO job.
         * Generally, an action involves at least one input or output artifact. For more
         * information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/lineage-tracking.html">Amazon
         * SageMaker ML Lineage Tracking</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateAction">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateActionOutcome CreateAction(const Model::CreateActionRequest& request) const;


        /**
         * <p>Create a machine learning algorithm that you can use in SageMaker and list in
         * the Amazon Web Services Marketplace.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateAlgorithm">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateAlgorithmOutcome CreateAlgorithm(const Model::CreateAlgorithmRequest& request) const;


        /**
         * <p>Creates a running app for the specified UserProfile. This operation is
         * automatically invoked by Amazon SageMaker Studio upon access to the associated
         * Domain, and when new kernel configurations are selected by the user. A user may
         * have multiple Apps active simultaneously.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateApp">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateAppOutcome CreateApp(const Model::CreateAppRequest& request) const;


        /**
         * <p>Creates a configuration for running a SageMaker image as a KernelGateway app.
         * The configuration specifies the Amazon Elastic File System (EFS) storage volume
         * on the image, and a list of the kernels in the image.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateAppImageConfig">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateAppImageConfigOutcome CreateAppImageConfig(const Model::CreateAppImageConfigRequest& request) const;


        /**
         * <p>Creates an <i>artifact</i>. An artifact is a lineage tracking entity that
         * represents a URI addressable object or data. Some examples are the S3 URI of a
         * dataset and the ECR registry path of an image. For more information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/lineage-tracking.html">Amazon
         * SageMaker ML Lineage Tracking</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateArtifact">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateArtifactOutcome CreateArtifact(const Model::CreateArtifactRequest& request) const;


        /**
         * <p>Creates an Autopilot job.</p> <p>Find the best-performing model after you run
         * an Autopilot job by calling .</p> <p>For information about how to use Autopilot,
         * see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/autopilot-automate-model-development.html">Automate
         * Model Development with Amazon SageMaker Autopilot</a>.</p><p><h3>See Also:</h3> 
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateAutoMLJob">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateAutoMLJobOutcome CreateAutoMLJob(const Model::CreateAutoMLJobRequest& request) const;


        /**
         * <p>Creates a Git repository as a resource in your SageMaker account. You can
         * associate the repository with notebook instances so that you can use Git source
         * control for the notebooks you create. The Git repository is a resource in your
         * SageMaker account, so it can be associated with more than one notebook instance,
         * and it persists independently from the lifecycle of any notebook instances it is
         * associated with.</p> <p>The repository can be hosted either in <a
         * href="https://docs.aws.amazon.com/codecommit/latest/userguide/welcome.html">Amazon
         * Web Services CodeCommit</a> or in any other Git repository.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateCodeRepository">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateCodeRepositoryOutcome CreateCodeRepository(const Model::CreateCodeRepositoryRequest& request) const;


        /**
         * <p>Starts a model compilation job. After the model has been compiled, Amazon
         * SageMaker saves the resulting model artifacts to an Amazon Simple Storage
         * Service (Amazon S3) bucket that you specify. </p> <p>If you choose to host your
         * model using Amazon SageMaker hosting services, you can use the resulting model
         * artifacts as part of the model. You can also use the artifacts with Amazon Web
         * Services IoT Greengrass. In that case, deploy them as an ML resource.</p> <p>In
         * the request body, you provide the following:</p> <ul> <li> <p>A name for the
         * compilation job</p> </li> <li> <p> Information about the input model artifacts
         * </p> </li> <li> <p>The output location for the compiled model and the device
         * (target) that the model runs on </p> </li> <li> <p>The Amazon Resource Name
         * (ARN) of the IAM role that Amazon SageMaker assumes to perform the model
         * compilation job. </p> </li> </ul> <p>You can also provide a <code>Tag</code> to
         * track the model compilation job's resource use and costs. The response body
         * contains the <code>CompilationJobArn</code> for the compiled job.</p> <p>To stop
         * a model compilation job, use <a>StopCompilationJob</a>. To get information about
         * a particular model compilation job, use <a>DescribeCompilationJob</a>. To get
         * information about multiple model compilation jobs, use
         * <a>ListCompilationJobs</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateCompilationJob">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateCompilationJobOutcome CreateCompilationJob(const Model::CreateCompilationJobRequest& request) const;


        /**
         * <p>Creates a <i>context</i>. A context is a lineage tracking entity that
         * represents a logical grouping of other tracking or experiment entities. Some
         * examples are an endpoint and a model package. For more information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/lineage-tracking.html">Amazon
         * SageMaker ML Lineage Tracking</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateContext">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateContextOutcome CreateContext(const Model::CreateContextRequest& request) const;


        /**
         * <p>Creates a definition for a job that monitors data quality and drift. For
         * information about model monitor, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/model-monitor.html">Amazon
         * SageMaker Model Monitor</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateDataQualityJobDefinition">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateDataQualityJobDefinitionOutcome CreateDataQualityJobDefinition(const Model::CreateDataQualityJobDefinitionRequest& request) const;


        /**
         * <p>Creates a device fleet.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateDeviceFleet">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateDeviceFleetOutcome CreateDeviceFleet(const Model::CreateDeviceFleetRequest& request) const;


        /**
         * <p>Creates a <code>Domain</code> used by Amazon SageMaker Studio. A domain
         * consists of an associated Amazon Elastic File System (EFS) volume, a list of
         * authorized users, and a variety of security, application, policy, and Amazon
         * Virtual Private Cloud (VPC) configurations. An Amazon Web Services account is
         * limited to one domain per region. Users within a domain can share notebook files
         * and other artifacts with each other.</p> <p> <b>EFS storage</b> </p> <p>When a
         * domain is created, an EFS volume is created for use by all of the users within
         * the domain. Each user receives a private home directory within the EFS volume
         * for notebooks, Git repositories, and data files.</p> <p>SageMaker uses the
         * Amazon Web Services Key Management Service (Amazon Web Services KMS) to encrypt
         * the EFS volume attached to the domain with an Amazon Web Services managed key by
         * default. For more control, you can specify a customer managed key. For more
         * information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/encryption-at-rest.html">Protect
         * Data at Rest Using Encryption</a>.</p> <p> <b>VPC configuration</b> </p> <p>All
         * SageMaker Studio traffic between the domain and the EFS volume is through the
         * specified VPC and subnets. For other Studio traffic, you can specify the
         * <code>AppNetworkAccessType</code> parameter. <code>AppNetworkAccessType</code>
         * corresponds to the network access type that you choose when you onboard to
         * Studio. The following options are available:</p> <ul> <li> <p>
         * <code>PublicInternetOnly</code> - Non-EFS traffic goes through a VPC managed by
         * Amazon SageMaker, which allows internet access. This is the default value.</p>
         * </li> <li> <p> <code>VpcOnly</code> - All Studio traffic is through the
         * specified VPC and subnets. Internet access is disabled by default. To allow
         * internet access, you must specify a NAT gateway.</p> <p>When internet access is
         * disabled, you won't be able to run a Studio notebook or to train or host models
         * unless your VPC has an interface endpoint to the SageMaker API and runtime or a
         * NAT gateway and your security groups allow outbound connections.</p> </li> </ul>
         *  <p>NFS traffic over TCP on port 2049 needs to be allowed in both
         * inbound and outbound rules in order to launch a SageMaker Studio app
         * successfully.</p>  <p>For more information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/studio-notebooks-and-internet-access.html">Connect
         * SageMaker Studio Notebooks to Resources in a VPC</a>.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateDomain">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateDomainOutcome CreateDomain(const Model::CreateDomainRequest& request) const;


        /**
         * <p>Creates an edge deployment plan, consisting of multiple stages. Each stage
         * may have a different deployment configuration and devices.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateEdgeDeploymentPlan">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateEdgeDeploymentPlanOutcome CreateEdgeDeploymentPlan(const Model::CreateEdgeDeploymentPlanRequest& request) const;


        /**
         * <p>Creates a new stage in an existing edge deployment plan.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateEdgeDeploymentStage">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateEdgeDeploymentStageOutcome CreateEdgeDeploymentStage(const Model::CreateEdgeDeploymentStageRequest& request) const;


        /**
         * <p>Starts a SageMaker Edge Manager model packaging job. Edge Manager will use
         * the model artifacts from the Amazon Simple Storage Service bucket that you
         * specify. After the model has been packaged, Amazon SageMaker saves the resulting
         * artifacts to an S3 bucket that you specify.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateEdgePackagingJob">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateEdgePackagingJobOutcome CreateEdgePackagingJob(const Model::CreateEdgePackagingJobRequest& request) const;


        /**
         * <p>Creates an endpoint using the endpoint configuration specified in the
         * request. SageMaker uses the endpoint to provision resources and deploy models.
         * You create the endpoint configuration with the <a>CreateEndpointConfig</a> API.
         * </p> <p> Use this API to deploy models using SageMaker hosting services. </p>
         * <p>For an example that calls this method when deploying a model to SageMaker
         * hosting services, see the <a
         * href="https://github.com/aws/amazon-sagemaker-examples/blob/master/sagemaker-fundamentals/create-endpoint/create_endpoint.ipynb">Create
         * Endpoint example notebook.</a> </p>  <p> You must not delete an
         * <code>EndpointConfig</code> that is in use by an endpoint that is live or while
         * the <code>UpdateEndpoint</code> or <code>CreateEndpoint</code> operations are
         * being performed on the endpoint. To update an endpoint, you must create a new
         * <code>EndpointConfig</code>.</p>  <p>The endpoint name must be unique
         * within an Amazon Web Services Region in your Amazon Web Services account. </p>
         * <p>When it receives the request, SageMaker creates the endpoint, launches the
         * resources (ML compute instances), and deploys the model(s) on them. </p> 
         * <p>When you call <a>CreateEndpoint</a>, a load call is made to DynamoDB to
         * verify that your endpoint configuration exists. When you read data from a
         * DynamoDB table supporting <a
         * href="https://docs.aws.amazon.com/amazondynamodb/latest/developerguide/HowItWorks.ReadConsistency.html">
         * <code>Eventually Consistent Reads</code> </a>, the response might not reflect
         * the results of a recently completed write operation. The response might include
         * some stale data. If the dependent entities are not yet in DynamoDB, this causes
         * a validation error. If you repeat your read request after a short time, the
         * response should return the latest data. So retry logic is recommended to handle
         * these possible issues. We also recommend that customers call
         * <a>DescribeEndpointConfig</a> before calling <a>CreateEndpoint</a> to minimize
         * the potential impact of a DynamoDB eventually consistent read.</p> 
         * <p>When SageMaker receives the request, it sets the endpoint status to
         * <code>Creating</code>. After it creates the endpoint, it sets the status to
         * <code>InService</code>. SageMaker can then process incoming requests for
         * inferences. To check the status of an endpoint, use the <a>DescribeEndpoint</a>
         * API.</p> <p>If any of the models hosted at this endpoint get model data from an
         * Amazon S3 location, SageMaker uses Amazon Web Services Security Token Service to
         * download model artifacts from the S3 path you provided. Amazon Web Services STS
         * is activated in your IAM user account by default. If you previously deactivated
         * Amazon Web Services STS for a region, you need to reactivate Amazon Web Services
         * STS for that region. For more information, see <a
         * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_credentials_temp_enable-regions.html">Activating
         * and Deactivating Amazon Web Services STS in an Amazon Web Services Region</a> in
         * the <i>Amazon Web Services Identity and Access Management User Guide</i>.</p>
         *  <p> To add the IAM role policies for using this API operation, go to the
         * <a href="https://console.aws.amazon.com/iam/">IAM console</a>, and choose Roles
         * in the left navigation pane. Search the IAM role that you want to grant access
         * to use the <a>CreateEndpoint</a> and <a>CreateEndpointConfig</a> API operations,
         * add the following policies to the role. </p> <ul> <li> <p>Option 1: For a full
         * SageMaker access, search and attach the <code>AmazonSageMakerFullAccess</code>
         * policy.</p> </li> <li> <p>Option 2: For granting a limited access to an IAM
         * role, paste the following Action elements manually into the JSON file of the IAM
         * role: </p> <p> <code>"Action": ["sagemaker:CreateEndpoint",
         * "sagemaker:CreateEndpointConfig"]</code> </p> <p> <code>"Resource": [</code>
         * </p> <p>
         * <code>"arn:aws:sagemaker:region:account-id:endpoint/endpointName"</code> </p>
         * <p>
         * <code>"arn:aws:sagemaker:region:account-id:endpoint-config/endpointConfigName"</code>
         * </p> <p> <code>]</code> </p> <p>For more information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/api-permissions-reference.html">SageMaker
         * API Permissions: Actions, Permissions, and Resources Reference</a>.</p> </li>
         * </ul> <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateEndpoint">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateEndpointOutcome CreateEndpoint(const Model::CreateEndpointRequest& request) const;


        /**
         * <p>Creates an endpoint configuration that SageMaker hosting services uses to
         * deploy models. In the configuration, you identify one or more models, created
         * using the <code>CreateModel</code> API, to deploy and the resources that you
         * want SageMaker to provision. Then you call the <a>CreateEndpoint</a> API.</p>
         *  <p> Use this API if you want to use SageMaker hosting services to deploy
         * models into production. </p>  <p>In the request, you define a
         * <code>ProductionVariant</code>, for each model that you want to deploy. Each
         * <code>ProductionVariant</code> parameter also describes the resources that you
         * want SageMaker to provision. This includes the number and type of ML compute
         * instances to deploy. </p> <p>If you are hosting multiple models, you also assign
         * a <code>VariantWeight</code> to specify how much traffic you want to allocate to
         * each model. For example, suppose that you want to host two models, A and B, and
         * you assign traffic weight 2 for model A and 1 for model B. SageMaker distributes
         * two-thirds of the traffic to Model A, and one-third to model B. </p> 
         * <p>When you call <a>CreateEndpoint</a>, a load call is made to DynamoDB to
         * verify that your endpoint configuration exists. When you read data from a
         * DynamoDB table supporting <a
         * href="https://docs.aws.amazon.com/amazondynamodb/latest/developerguide/HowItWorks.ReadConsistency.html">
         * <code>Eventually Consistent Reads</code> </a>, the response might not reflect
         * the results of a recently completed write operation. The response might include
         * some stale data. If the dependent entities are not yet in DynamoDB, this causes
         * a validation error. If you repeat your read request after a short time, the
         * response should return the latest data. So retry logic is recommended to handle
         * these possible issues. We also recommend that customers call
         * <a>DescribeEndpointConfig</a> before calling <a>CreateEndpoint</a> to minimize
         * the potential impact of a DynamoDB eventually consistent read.</p>
         * <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateEndpointConfig">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateEndpointConfigOutcome CreateEndpointConfig(const Model::CreateEndpointConfigRequest& request) const;


        /**
         * <p>Creates an SageMaker <i>experiment</i>. An experiment is a collection of
         * <i>trials</i> that are observed, compared and evaluated as a group. A trial is a
         * set of steps, called <i>trial components</i>, that produce a machine learning
         * model.</p> <p>The goal of an experiment is to determine the components that
         * produce the best model. Multiple trials are performed, each one isolating and
         * measuring the impact of a change to one or more inputs, while keeping the
         * remaining inputs constant.</p> <p>When you use SageMaker Studio or the SageMaker
         * Python SDK, all experiments, trials, and trial components are automatically
         * tracked, logged, and indexed. When you use the Amazon Web Services SDK for
         * Python (Boto), you must use the logging APIs provided by the SDK.</p> <p>You can
         * add tags to experiments, trials, trial components and then use the <a>Search</a>
         * API to search for the tags.</p> <p>To add a description to an experiment,
         * specify the optional <code>Description</code> parameter. To add a description
         * later, or to change the description, call the <a>UpdateExperiment</a> API.</p>
         * <p>To get a list of all your experiments, call the <a>ListExperiments</a> API.
         * To view an experiment's properties, call the <a>DescribeExperiment</a> API. To
         * get a list of all the trials associated with an experiment, call the
         * <a>ListTrials</a> API. To create a trial call the <a>CreateTrial</a>
         * API.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateExperiment">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateExperimentOutcome CreateExperiment(const Model::CreateExperimentRequest& request) const;


        /**
         * <p>Create a new <code>FeatureGroup</code>. A <code>FeatureGroup</code> is a
         * group of <code>Features</code> defined in the <code>FeatureStore</code> to
         * describe a <code>Record</code>. </p> <p>The <code>FeatureGroup</code> defines
         * the schema and features contained in the FeatureGroup. A
         * <code>FeatureGroup</code> definition is composed of a list of
         * <code>Features</code>, a <code>RecordIdentifierFeatureName</code>, an
         * <code>EventTimeFeatureName</code> and configurations for its
         * <code>OnlineStore</code> and <code>OfflineStore</code>. Check <a
         * href="https://docs.aws.amazon.com/general/latest/gr/aws_service_limits.html">Amazon
         * Web Services service quotas</a> to see the <code>FeatureGroup</code>s quota for
         * your Amazon Web Services account.</p>  <p>You must include at least
         * one of <code>OnlineStoreConfig</code> and <code>OfflineStoreConfig</code> to
         * create a <code>FeatureGroup</code>.</p> <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateFeatureGroup">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateFeatureGroupOutcome CreateFeatureGroup(const Model::CreateFeatureGroupRequest& request) const;


        /**
         * <p>Creates a flow definition.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateFlowDefinition">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateFlowDefinitionOutcome CreateFlowDefinition(const Model::CreateFlowDefinitionRequest& request) const;


        /**
         * <p>Defines the settings you will use for the human review workflow user
         * interface. Reviewers will see a three-panel interface with an instruction area,
         * the item to review, and an input area.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateHumanTaskUi">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateHumanTaskUiOutcome CreateHumanTaskUi(const Model::CreateHumanTaskUiRequest& request) const;


        /**
         * <p>Starts a hyperparameter tuning job. A hyperparameter tuning job finds the
         * best version of a model by running many training jobs on your dataset using the
         * algorithm you choose and values for hyperparameters within ranges that you
         * specify. It then chooses the hyperparameter values that result in a model that
         * performs the best, as measured by an objective metric that you choose.</p> <p>A
         * hyperparameter tuning job automatically creates Amazon SageMaker experiments,
         * trials, and trial components for each training job that it runs. You can view
         * these entities in Amazon SageMaker Studio. For more information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/experiments-view-compare.html#experiments-view">View
         * Experiments, Trials, and Trial Components</a>.</p>  <p>Do not include
         * any security-sensitive information including account access IDs, secrets or
         * tokens in any hyperparameter field. If the use of security-sensitive credentials
         * are detected, SageMaker will reject your training job request and return an
         * exception error.</p> <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateHyperParameterTuningJob">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateHyperParameterTuningJobOutcome CreateHyperParameterTuningJob(const Model::CreateHyperParameterTuningJobRequest& request) const;


        /**
         * <p>Creates a custom SageMaker image. A SageMaker image is a set of image
         * versions. Each image version represents a container image stored in Amazon
         * Elastic Container Registry (ECR). For more information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/studio-byoi.html">Bring
         * your own SageMaker image</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateImage">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateImageOutcome CreateImage(const Model::CreateImageRequest& request) const;


        /**
         * <p>Creates a version of the SageMaker image specified by <code>ImageName</code>.
         * The version represents the Amazon Elastic Container Registry (ECR) container
         * image specified by <code>BaseImage</code>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateImageVersion">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateImageVersionOutcome CreateImageVersion(const Model::CreateImageVersionRequest& request) const;


        /**
         * <p>Starts a recommendation job. You can create either an instance recommendation
         * or load test job.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateInferenceRecommendationsJob">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateInferenceRecommendationsJobOutcome CreateInferenceRecommendationsJob(const Model::CreateInferenceRecommendationsJobRequest& request) const;


        /**
         * <p>Creates a job that uses workers to label the data objects in your input
         * dataset. You can use the labeled data to train machine learning models. </p>
         * <p>You can select your workforce from one of three providers:</p> <ul> <li> <p>A
         * private workforce that you create. It can include employees, contractors, and
         * outside experts. Use a private workforce when want the data to stay within your
         * organization or when a specific set of skills is required.</p> </li> <li> <p>One
         * or more vendors that you select from the Amazon Web Services Marketplace.
         * Vendors provide expertise in specific areas. </p> </li> <li> <p>The Amazon
         * Mechanical Turk workforce. This is the largest workforce, but it should only be
         * used for public data or data that has been stripped of any personally
         * identifiable information.</p> </li> </ul> <p>You can also use <i>automated data
         * labeling</i> to reduce the number of data objects that need to be labeled by a
         * human. Automated data labeling uses <i>active learning</i> to determine if a
         * data object can be labeled by machine or if it needs to be sent to a human
         * worker. For more information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/sms-automated-labeling.html">Using
         * Automated Data Labeling</a>.</p> <p>The data objects to be labeled are contained
         * in an Amazon S3 bucket. You create a <i>manifest file</i> that describes the
         * location of each object. For more information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/sms-data.html">Using Input
         * and Output Data</a>.</p> <p>The output can be used as the manifest file for
         * another labeling job or as training data for your machine learning models.</p>
         * <p>You can use this operation to create a static labeling job or a streaming
         * labeling job. A static labeling job stops if all data objects in the input
         * manifest file identified in <code>ManifestS3Uri</code> have been labeled. A
         * streaming labeling job runs perpetually until it is manually stopped, or remains
         * idle for 10 days. You can send new data objects to an active
         * (<code>InProgress</code>) streaming labeling job in real time. To learn how to
         * create a static labeling job, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/sms-create-labeling-job-api.html">Create
         * a Labeling Job (API) </a> in the Amazon SageMaker Developer Guide. To learn how
         * to create a streaming labeling job, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/sms-streaming-create-job.html">Create
         * a Streaming Labeling Job</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateLabelingJob">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateLabelingJobOutcome CreateLabelingJob(const Model::CreateLabelingJobRequest& request) const;


        /**
         * <p>Creates a model in SageMaker. In the request, you name the model and describe
         * a primary container. For the primary container, you specify the Docker image
         * that contains inference code, artifacts (from prior training), and a custom
         * environment map that the inference code uses when you deploy the model for
         * predictions.</p> <p>Use this API to create a model if you want to use SageMaker
         * hosting services or run a batch transform job.</p> <p>To host your model, you
         * create an endpoint configuration with the <code>CreateEndpointConfig</code> API,
         * and then create an endpoint with the <code>CreateEndpoint</code> API. SageMaker
         * then deploys all of the containers that you defined for the model in the hosting
         * environment. </p> <p>For an example that calls this method when deploying a
         * model to SageMaker hosting services, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/realtime-endpoints-deployment.html#realtime-endpoints-deployment-create-model">Create
         * a Model (Amazon Web Services SDK for Python (Boto 3)).</a> </p> <p>To run a
         * batch transform using your model, you start a job with the
         * <code>CreateTransformJob</code> API. SageMaker uses your model and your dataset
         * to get inferences which are then saved to a specified S3 location.</p> <p>In the
         * request, you also provide an IAM role that SageMaker can assume to access model
         * artifacts and docker image for deployment on ML compute hosting instances or for
         * batch transform jobs. In addition, you also use the IAM role to manage
         * permissions the inference code needs. For example, if the inference code access
         * any other Amazon Web Services resources, you grant necessary permissions via
         * this role.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateModel">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateModelOutcome CreateModel(const Model::CreateModelRequest& request) const;


        /**
         * <p>Creates the definition for a model bias job.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateModelBiasJobDefinition">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateModelBiasJobDefinitionOutcome CreateModelBiasJobDefinition(const Model::CreateModelBiasJobDefinitionRequest& request) const;


        /**
         * <p>Creates the definition for a model explainability job.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateModelExplainabilityJobDefinition">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateModelExplainabilityJobDefinitionOutcome CreateModelExplainabilityJobDefinition(const Model::CreateModelExplainabilityJobDefinitionRequest& request) const;


        /**
         * <p>Creates a model package that you can use to create SageMaker models or list
         * on Amazon Web Services Marketplace, or a versioned model that is part of a model
         * group. Buyers can subscribe to model packages listed on Amazon Web Services
         * Marketplace to create models in SageMaker.</p> <p>To create a model package by
         * specifying a Docker container that contains your inference code and the Amazon
         * S3 location of your model artifacts, provide values for
         * <code>InferenceSpecification</code>. To create a model from an algorithm
         * resource that you created or subscribed to in Amazon Web Services Marketplace,
         * provide a value for <code>SourceAlgorithmSpecification</code>.</p> 
         * <p>There are two types of model packages:</p> <ul> <li> <p>Versioned - a model
         * that is part of a model group in the model registry.</p> </li> <li>
         * <p>Unversioned - a model package that is not part of a model group.</p> </li>
         * </ul> <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateModelPackage">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateModelPackageOutcome CreateModelPackage(const Model::CreateModelPackageRequest& request) const;


        /**
         * <p>Creates a model group. A model group contains a group of model
         * versions.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateModelPackageGroup">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateModelPackageGroupOutcome CreateModelPackageGroup(const Model::CreateModelPackageGroupRequest& request) const;


        /**
         * <p>Creates a definition for a job that monitors model quality and drift. For
         * information about model monitor, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/model-monitor.html">Amazon
         * SageMaker Model Monitor</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateModelQualityJobDefinition">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateModelQualityJobDefinitionOutcome CreateModelQualityJobDefinition(const Model::CreateModelQualityJobDefinitionRequest& request) const;


        /**
         * <p>Creates a schedule that regularly starts Amazon SageMaker Processing Jobs to
         * monitor the data captured for an Amazon SageMaker Endoint.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateMonitoringSchedule">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateMonitoringScheduleOutcome CreateMonitoringSchedule(const Model::CreateMonitoringScheduleRequest& request) const;


        /**
         * <p>Creates an SageMaker notebook instance. A notebook instance is a machine
         * learning (ML) compute instance running on a Jupyter notebook. </p> <p>In a
         * <code>CreateNotebookInstance</code> request, specify the type of ML compute
         * instance that you want to run. SageMaker launches the instance, installs common
         * libraries that you can use to explore datasets for model training, and attaches
         * an ML storage volume to the notebook instance. </p> <p>SageMaker also provides a
         * set of example notebooks. Each notebook demonstrates how to use SageMaker with a
         * specific algorithm or with a machine learning framework. </p> <p>After receiving
         * the request, SageMaker does the following:</p> <ol> <li> <p>Creates a network
         * interface in the SageMaker VPC.</p> </li> <li> <p>(Option) If you specified
         * <code>SubnetId</code>, SageMaker creates a network interface in your own VPC,
         * which is inferred from the subnet ID that you provide in the input. When
         * creating this network interface, SageMaker attaches the security group that you
         * specified in the request to the network interface that it creates in your
         * VPC.</p> </li> <li> <p>Launches an EC2 instance of the type specified in the
         * request in the SageMaker VPC. If you specified <code>SubnetId</code> of your
         * VPC, SageMaker specifies both network interfaces when launching this instance.
         * This enables inbound traffic from your own VPC to the notebook instance,
         * assuming that the security groups allow it.</p> </li> </ol> <p>After creating
         * the notebook instance, SageMaker returns its Amazon Resource Name (ARN). You
         * can't change the name of a notebook instance after you create it.</p> <p>After
         * SageMaker creates the notebook instance, you can connect to the Jupyter server
         * and work in Jupyter notebooks. For example, you can write code to explore a
         * dataset that you can use for model training, train a model, host models by
         * creating SageMaker endpoints, and validate hosted models. </p> <p>For more
         * information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/how-it-works.html">How It
         * Works</a>. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateNotebookInstance">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateNotebookInstanceOutcome CreateNotebookInstance(const Model::CreateNotebookInstanceRequest& request) const;


        /**
         * <p>Creates a lifecycle configuration that you can associate with a notebook
         * instance. A <i>lifecycle configuration</i> is a collection of shell scripts that
         * run when you create or start a notebook instance.</p> <p>Each lifecycle
         * configuration script has a limit of 16384 characters.</p> <p>The value of the
         * <code>$PATH</code> environment variable that is available to both scripts is
         * <code>/sbin:bin:/usr/sbin:/usr/bin</code>.</p> <p>View CloudWatch Logs for
         * notebook instance lifecycle configurations in log group
         * <code>/aws/sagemaker/NotebookInstances</code> in log stream
         * <code>[notebook-instance-name]/[LifecycleConfigHook]</code>.</p> <p>Lifecycle
         * configuration scripts cannot run for longer than 5 minutes. If a script runs for
         * longer than 5 minutes, it fails and the notebook instance is not created or
         * started.</p> <p>For information about notebook instance lifestyle
         * configurations, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/notebook-lifecycle-config.html">Step
         * 2.1: (Optional) Customize a Notebook Instance</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateNotebookInstanceLifecycleConfig">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateNotebookInstanceLifecycleConfigOutcome CreateNotebookInstanceLifecycleConfig(const Model::CreateNotebookInstanceLifecycleConfigRequest& request) const;


        /**
         * <p>Creates a pipeline using a JSON pipeline definition.</p><p><h3>See Also:</h3>
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreatePipeline">AWS
         * API Reference</a></p>
         */
        virtual Model::CreatePipelineOutcome CreatePipeline(const Model::CreatePipelineRequest& request) const;


        /**
         * <p>Creates a URL for a specified UserProfile in a Domain. When accessed in a web
         * browser, the user will be automatically signed in to Amazon SageMaker Studio,
         * and granted access to all of the Apps and files associated with the Domain's
         * Amazon Elastic File System (EFS) volume. This operation can only be called when
         * the authentication mode equals IAM. </p> <p>The IAM role or user passed to this
         * API defines the permissions to access the app. Once the presigned URL is
         * created, no additional permission is required to access this URL. IAM
         * authorization policies for this API are also enforced for every HTTP request and
         * WebSocket frame that attempts to connect to the app.</p> <p>You can restrict
         * access to this API and to the URL that it returns to a list of IP addresses,
         * Amazon VPCs or Amazon VPC Endpoints that you specify. For more information, see
         * <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/studio-interface-endpoint.html">Connect
         * to SageMaker Studio Through an Interface VPC Endpoint</a> .</p>  <p>The
         * URL that you get from a call to <code>CreatePresignedDomainUrl</code> has a
         * default timeout of 5 minutes. You can configure this value using
         * <code>ExpiresInSeconds</code>. If you try to use the URL after the timeout limit
         * expires, you are directed to the Amazon Web Services console sign-in page.</p>
         * <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreatePresignedDomainUrl">AWS
         * API Reference</a></p>
         */
        virtual Model::CreatePresignedDomainUrlOutcome CreatePresignedDomainUrl(const Model::CreatePresignedDomainUrlRequest& request) const;


        /**
         * <p>Returns a URL that you can use to connect to the Jupyter server from a
         * notebook instance. In the SageMaker console, when you choose <code>Open</code>
         * next to a notebook instance, SageMaker opens a new tab showing the Jupyter
         * server home page from the notebook instance. The console uses this API to get
         * the URL and show the page.</p> <p> The IAM role or user used to call this API
         * defines the permissions to access the notebook instance. Once the presigned URL
         * is created, no additional permission is required to access this URL. IAM
         * authorization policies for this API are also enforced for every HTTP request and
         * WebSocket frame that attempts to connect to the notebook instance.</p> <p>You
         * can restrict access to this API and to the URL that it returns to a list of IP
         * addresses that you specify. Use the <code>NotIpAddress</code> condition operator
         * and the <code>aws:SourceIP</code> condition context key to specify the list of
         * IP addresses that you want to have access to the notebook instance. For more
         * information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/security_iam_id-based-policy-examples.html#nbi-ip-filter">Limit
         * Access to a Notebook Instance by IP Address</a>.</p>  <p>The URL that you
         * get from a call to <a>CreatePresignedNotebookInstanceUrl</a> is valid only for 5
         * minutes. If you try to use the URL after the 5-minute limit expires, you are
         * directed to the Amazon Web Services console sign-in page.</p> <p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreatePresignedNotebookInstanceUrl">AWS
         * API Reference</a></p>
         */
        virtual Model::CreatePresignedNotebookInstanceUrlOutcome CreatePresignedNotebookInstanceUrl(const Model::CreatePresignedNotebookInstanceUrlRequest& request) const;


        /**
         * <p>Creates a processing job.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateProcessingJob">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateProcessingJobOutcome CreateProcessingJob(const Model::CreateProcessingJobRequest& request) const;


        /**
         * <p>Creates a machine learning (ML) project that can contain one or more
         * templates that set up an ML pipeline from training to deploying an approved
         * model.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateProject">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateProjectOutcome CreateProject(const Model::CreateProjectRequest& request) const;


        /**
         * <p>Creates a new Studio Lifecycle Configuration.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateStudioLifecycleConfig">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateStudioLifecycleConfigOutcome CreateStudioLifecycleConfig(const Model::CreateStudioLifecycleConfigRequest& request) const;


        /**
         * <p>Starts a model training job. After training completes, SageMaker saves the
         * resulting model artifacts to an Amazon S3 location that you specify. </p> <p>If
         * you choose to host your model using SageMaker hosting services, you can use the
         * resulting model artifacts as part of the model. You can also use the artifacts
         * in a machine learning service other than SageMaker, provided that you know how
         * to use them for inference. </p> <p>In the request body, you provide the
         * following: </p> <ul> <li> <p> <code>AlgorithmSpecification</code> - Identifies
         * the training algorithm to use. </p> </li> <li> <p> <code>HyperParameters</code>
         * - Specify these algorithm-specific parameters to enable the estimation of model
         * parameters during training. Hyperparameters can be tuned to optimize this
         * learning process. For a list of hyperparameters for each training algorithm
         * provided by SageMaker, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/algos.html">Algorithms</a>.
         * </p>  <p>Do not include any security-sensitive information including
         * account access IDs, secrets or tokens in any hyperparameter field. If the use of
         * security-sensitive credentials are detected, SageMaker will reject your training
         * job request and return an exception error.</p>  </li> <li> <p>
         * <code>InputDataConfig</code> - Describes the input required by the training job
         * and the Amazon S3, EFS, or FSx location where it is stored.</p> </li> <li> <p>
         * <code>OutputDataConfig</code> - Identifies the Amazon S3 bucket where you want
         * SageMaker to save the results of model training. </p> </li> <li> <p>
         * <code>ResourceConfig</code> - Identifies the resources, ML compute instances,
         * and ML storage volumes to deploy for model training. In distributed training,
         * you specify more than one instance. </p> </li> <li> <p>
         * <code>EnableManagedSpotTraining</code> - Optimize the cost of training machine
         * learning models by up to 80% by using Amazon EC2 Spot instances. For more
         * information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/model-managed-spot-training.html">Managed
         * Spot Training</a>. </p> </li> <li> <p> <code>RoleArn</code> - The Amazon
         * Resource Name (ARN) that SageMaker assumes to perform tasks on your behalf
         * during model training. You must grant this role the necessary permissions so
         * that SageMaker can successfully complete model training. </p> </li> <li> <p>
         * <code>StoppingCondition</code> - To help cap training costs, use
         * <code>MaxRuntimeInSeconds</code> to set a time limit for training. Use
         * <code>MaxWaitTimeInSeconds</code> to specify how long a managed spot training
         * job has to complete. </p> </li> <li> <p> <code>Environment</code> - The
         * environment variables to set in the Docker container.</p> </li> <li> <p>
         * <code>RetryStrategy</code> - The number of times to retry the job when the job
         * fails due to an <code>InternalServerError</code>.</p> </li> </ul> <p> For more
         * information about SageMaker, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/how-it-works.html">How It
         * Works</a>. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateTrainingJob">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateTrainingJobOutcome CreateTrainingJob(const Model::CreateTrainingJobRequest& request) const;


        /**
         * <p>Starts a transform job. A transform job uses a trained model to get
         * inferences on a dataset and saves these results to an Amazon S3 location that
         * you specify.</p> <p>To perform batch transformations, you create a transform job
         * and use the data that you have readily available.</p> <p>In the request body,
         * you provide the following:</p> <ul> <li> <p> <code>TransformJobName</code> -
         * Identifies the transform job. The name must be unique within an Amazon Web
         * Services Region in an Amazon Web Services account.</p> </li> <li> <p>
         * <code>ModelName</code> - Identifies the model to use. <code>ModelName</code>
         * must be the name of an existing Amazon SageMaker model in the same Amazon Web
         * Services Region and Amazon Web Services account. For information on creating a
         * model, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/APIReference/API_CreateModel.html">CreateModel</a>.</p>
         * </li> <li> <p> <code>TransformInput</code> - Describes the dataset to be
         * transformed and the Amazon S3 location where it is stored.</p> </li> <li> <p>
         * <code>TransformOutput</code> - Identifies the Amazon S3 location where you want
         * Amazon SageMaker to save the results from the transform job.</p> </li> <li> <p>
         * <code>TransformResources</code> - Identifies the ML compute instances for the
         * transform job.</p> </li> </ul> <p>For more information about how batch
         * transformation works, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/batch-transform.html">Batch
         * Transform</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateTransformJob">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateTransformJobOutcome CreateTransformJob(const Model::CreateTransformJobRequest& request) const;


        /**
         * <p>Creates an SageMaker <i>trial</i>. A trial is a set of steps called <i>trial
         * components</i> that produce a machine learning model. A trial is part of a
         * single SageMaker <i>experiment</i>.</p> <p>When you use SageMaker Studio or the
         * SageMaker Python SDK, all experiments, trials, and trial components are
         * automatically tracked, logged, and indexed. When you use the Amazon Web Services
         * SDK for Python (Boto), you must use the logging APIs provided by the SDK.</p>
         * <p>You can add tags to a trial and then use the <a>Search</a> API to search for
         * the tags.</p> <p>To get a list of all your trials, call the <a>ListTrials</a>
         * API. To view a trial's properties, call the <a>DescribeTrial</a> API. To create
         * a trial component, call the <a>CreateTrialComponent</a> API.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateTrial">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateTrialOutcome CreateTrial(const Model::CreateTrialRequest& request) const;


        /**
         * <p>Creates a <i>trial component</i>, which is a stage of a machine learning
         * <i>trial</i>. A trial is composed of one or more trial components. A trial
         * component can be used in multiple trials.</p> <p>Trial components include
         * pre-processing jobs, training jobs, and batch transform jobs.</p> <p>When you
         * use SageMaker Studio or the SageMaker Python SDK, all experiments, trials, and
         * trial components are automatically tracked, logged, and indexed. When you use
         * the Amazon Web Services SDK for Python (Boto), you must use the logging APIs
         * provided by the SDK.</p> <p>You can add tags to a trial component and then use
         * the <a>Search</a> API to search for the tags.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateTrialComponent">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateTrialComponentOutcome CreateTrialComponent(const Model::CreateTrialComponentRequest& request) const;


        /**
         * <p>Creates a user profile. A user profile represents a single user within a
         * domain, and is the main way to reference a "person" for the purposes of sharing,
         * reporting, and other user-oriented features. This entity is created when a user
         * onboards to Amazon SageMaker Studio. If an administrator invites a person by
         * email or imports them from IAM Identity Center, a user profile is automatically
         * created. A user profile is the primary holder of settings for an individual user
         * and has a reference to the user's private Amazon Elastic File System (EFS) home
         * directory. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateUserProfile">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateUserProfileOutcome CreateUserProfile(const Model::CreateUserProfileRequest& request) const;


        /**
         * <p>Use this operation to create a workforce. This operation will return an error
         * if a workforce already exists in the Amazon Web Services Region that you
         * specify. You can only create one workforce in each Amazon Web Services Region
         * per Amazon Web Services account.</p> <p>If you want to create a new workforce in
         * an Amazon Web Services Region where a workforce already exists, use the API
         * operation to delete the existing workforce and then use
         * <code>CreateWorkforce</code> to create a new workforce.</p> <p>To create a
         * private workforce using Amazon Cognito, you must specify a Cognito user pool in
         * <code>CognitoConfig</code>. You can also create an Amazon Cognito workforce
         * using the Amazon SageMaker console. For more information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/sms-workforce-create-private.html">
         * Create a Private Workforce (Amazon Cognito)</a>.</p> <p>To create a private
         * workforce using your own OIDC Identity Provider (IdP), specify your IdP
         * configuration in <code>OidcConfig</code>. Your OIDC IdP must support
         * <i>groups</i> because groups are used by Ground Truth and Amazon A2I to create
         * work teams. For more information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/sms-workforce-create-private-oidc.html">
         * Create a Private Workforce (OIDC IdP)</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateWorkforce">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateWorkforceOutcome CreateWorkforce(const Model::CreateWorkforceRequest& request) const;


        /**
         * <p>Creates a new work team for labeling your data. A work team is defined by one
         * or more Amazon Cognito user pools. You must first create the user pools before
         * you can create a work team.</p> <p>You cannot create more than 25 work teams in
         * an account and region.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/CreateWorkteam">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateWorkteamOutcome CreateWorkteam(const Model::CreateWorkteamRequest& request) const;


        /**
         * <p>Deletes an action.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteAction">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteActionOutcome DeleteAction(const Model::DeleteActionRequest& request) const;


        /**
         * <p>Removes the specified algorithm from your account.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteAlgorithm">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteAlgorithmOutcome DeleteAlgorithm(const Model::DeleteAlgorithmRequest& request) const;


        /**
         * <p>Used to stop and delete an app.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteApp">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteAppOutcome DeleteApp(const Model::DeleteAppRequest& request) const;


        /**
         * <p>Deletes an AppImageConfig.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteAppImageConfig">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteAppImageConfigOutcome DeleteAppImageConfig(const Model::DeleteAppImageConfigRequest& request) const;


        /**
         * <p>Deletes an artifact. Either <code>ArtifactArn</code> or <code>Source</code>
         * must be specified.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteArtifact">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteArtifactOutcome DeleteArtifact(const Model::DeleteArtifactRequest& request) const;


        /**
         * <p>Deletes an association.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteAssociation">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteAssociationOutcome DeleteAssociation(const Model::DeleteAssociationRequest& request) const;


        /**
         * <p>Deletes the specified Git repository from your account.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteCodeRepository">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteCodeRepositoryOutcome DeleteCodeRepository(const Model::DeleteCodeRepositoryRequest& request) const;


        /**
         * <p>Deletes an context.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteContext">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteContextOutcome DeleteContext(const Model::DeleteContextRequest& request) const;


        /**
         * <p>Deletes a data quality monitoring job definition.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteDataQualityJobDefinition">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteDataQualityJobDefinitionOutcome DeleteDataQualityJobDefinition(const Model::DeleteDataQualityJobDefinitionRequest& request) const;


        /**
         * <p>Deletes a fleet.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteDeviceFleet">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteDeviceFleetOutcome DeleteDeviceFleet(const Model::DeleteDeviceFleetRequest& request) const;


        /**
         * <p>Used to delete a domain. If you onboarded with IAM mode, you will need to
         * delete your domain to onboard again using IAM Identity Center. Use with caution.
         * All of the members of the domain will lose access to their EFS volume, including
         * data, notebooks, and other artifacts. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteDomain">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteDomainOutcome DeleteDomain(const Model::DeleteDomainRequest& request) const;


        /**
         * <p>Deletes an edge deployment plan if (and only if) all the stages in the plan
         * are inactive or there are no stages in the plan.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteEdgeDeploymentPlan">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteEdgeDeploymentPlanOutcome DeleteEdgeDeploymentPlan(const Model::DeleteEdgeDeploymentPlanRequest& request) const;


        /**
         * <p>Delete a stage in an edge deployment plan if (and only if) the stage is
         * inactive.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteEdgeDeploymentStage">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteEdgeDeploymentStageOutcome DeleteEdgeDeploymentStage(const Model::DeleteEdgeDeploymentStageRequest& request) const;


        /**
         * <p>Deletes an endpoint. SageMaker frees up all of the resources that were
         * deployed when the endpoint was created. </p> <p>SageMaker retires any custom KMS
         * key grants associated with the endpoint, meaning you don't need to use the <a
         * href="http://docs.aws.amazon.com/kms/latest/APIReference/API_RevokeGrant.html">RevokeGrant</a>
         * API call.</p> <p>When you delete your endpoint, SageMaker asynchronously deletes
         * associated endpoint resources such as KMS key grants. You might still see these
         * resources in your account for a few minutes after deleting your endpoint. Do not
         * delete or revoke the permissions for your <code> <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/APIReference/API_CreateModel.html#sagemaker-CreateModel-request-ExecutionRoleArn">ExecutionRoleArn</a>
         * </code>, otherwise SageMaker cannot delete these resources.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteEndpoint">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteEndpointOutcome DeleteEndpoint(const Model::DeleteEndpointRequest& request) const;


        /**
         * <p>Deletes an endpoint configuration. The <code>DeleteEndpointConfig</code> API
         * deletes only the specified configuration. It does not delete endpoints created
         * using the configuration. </p> <p>You must not delete an
         * <code>EndpointConfig</code> in use by an endpoint that is live or while the
         * <code>UpdateEndpoint</code> or <code>CreateEndpoint</code> operations are being
         * performed on the endpoint. If you delete the <code>EndpointConfig</code> of an
         * endpoint that is active or being created or updated you may lose visibility into
         * the instance type the endpoint is using. The endpoint must be deleted in order
         * to stop incurring charges.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteEndpointConfig">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteEndpointConfigOutcome DeleteEndpointConfig(const Model::DeleteEndpointConfigRequest& request) const;


        /**
         * <p>Deletes an SageMaker experiment. All trials associated with the experiment
         * must be deleted first. Use the <a>ListTrials</a> API to get a list of the trials
         * associated with the experiment.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteExperiment">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteExperimentOutcome DeleteExperiment(const Model::DeleteExperimentRequest& request) const;


        /**
         * <p>Delete the <code>FeatureGroup</code> and any data that was written to the
         * <code>OnlineStore</code> of the <code>FeatureGroup</code>. Data cannot be
         * accessed from the <code>OnlineStore</code> immediately after
         * <code>DeleteFeatureGroup</code> is called. </p> <p>Data written into the
         * <code>OfflineStore</code> will not be deleted. The Amazon Web Services Glue
         * database and tables that are automatically created for your
         * <code>OfflineStore</code> are not deleted. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteFeatureGroup">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteFeatureGroupOutcome DeleteFeatureGroup(const Model::DeleteFeatureGroupRequest& request) const;


        /**
         * <p>Deletes the specified flow definition.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteFlowDefinition">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteFlowDefinitionOutcome DeleteFlowDefinition(const Model::DeleteFlowDefinitionRequest& request) const;


        /**
         * <p>Use this operation to delete a human task user interface (worker task
         * template).</p> <p> To see a list of human task user interfaces (work task
         * templates) in your account, use . When you delete a worker task template, it no
         * longer appears when you call <code>ListHumanTaskUis</code>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteHumanTaskUi">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteHumanTaskUiOutcome DeleteHumanTaskUi(const Model::DeleteHumanTaskUiRequest& request) const;


        /**
         * <p>Deletes a SageMaker image and all versions of the image. The container images
         * aren't deleted.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteImage">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteImageOutcome DeleteImage(const Model::DeleteImageRequest& request) const;


        /**
         * <p>Deletes a version of a SageMaker image. The container image the version
         * represents isn't deleted.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteImageVersion">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteImageVersionOutcome DeleteImageVersion(const Model::DeleteImageVersionRequest& request) const;


        /**
         * <p>Deletes a model. The <code>DeleteModel</code> API deletes only the model
         * entry that was created in SageMaker when you called the <code>CreateModel</code>
         * API. It does not delete model artifacts, inference code, or the IAM role that
         * you specified when creating the model. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteModel">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteModelOutcome DeleteModel(const Model::DeleteModelRequest& request) const;


        /**
         * <p>Deletes an Amazon SageMaker model bias job definition.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteModelBiasJobDefinition">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteModelBiasJobDefinitionOutcome DeleteModelBiasJobDefinition(const Model::DeleteModelBiasJobDefinitionRequest& request) const;


        /**
         * <p>Deletes an Amazon SageMaker model explainability job
         * definition.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteModelExplainabilityJobDefinition">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteModelExplainabilityJobDefinitionOutcome DeleteModelExplainabilityJobDefinition(const Model::DeleteModelExplainabilityJobDefinitionRequest& request) const;


        /**
         * <p>Deletes a model package.</p> <p>A model package is used to create SageMaker
         * models or list on Amazon Web Services Marketplace. Buyers can subscribe to model
         * packages listed on Amazon Web Services Marketplace to create models in
         * SageMaker.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteModelPackage">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteModelPackageOutcome DeleteModelPackage(const Model::DeleteModelPackageRequest& request) const;


        /**
         * <p>Deletes the specified model group.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteModelPackageGroup">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteModelPackageGroupOutcome DeleteModelPackageGroup(const Model::DeleteModelPackageGroupRequest& request) const;


        /**
         * <p>Deletes a model group resource policy.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteModelPackageGroupPolicy">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteModelPackageGroupPolicyOutcome DeleteModelPackageGroupPolicy(const Model::DeleteModelPackageGroupPolicyRequest& request) const;


        /**
         * <p>Deletes the secified model quality monitoring job definition.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteModelQualityJobDefinition">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteModelQualityJobDefinitionOutcome DeleteModelQualityJobDefinition(const Model::DeleteModelQualityJobDefinitionRequest& request) const;


        /**
         * <p>Deletes a monitoring schedule. Also stops the schedule had not already been
         * stopped. This does not delete the job execution history of the monitoring
         * schedule. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteMonitoringSchedule">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteMonitoringScheduleOutcome DeleteMonitoringSchedule(const Model::DeleteMonitoringScheduleRequest& request) const;


        /**
         * <p> Deletes an SageMaker notebook instance. Before you can delete a notebook
         * instance, you must call the <code>StopNotebookInstance</code> API. </p>
         *  <p>When you delete a notebook instance, you lose all of your data.
         * SageMaker removes the ML compute instance, and deletes the ML storage volume and
         * the network interface associated with the notebook instance. </p>
         * <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteNotebookInstance">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteNotebookInstanceOutcome DeleteNotebookInstance(const Model::DeleteNotebookInstanceRequest& request) const;


        /**
         * <p>Deletes a notebook instance lifecycle configuration.</p><p><h3>See Also:</h3>
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteNotebookInstanceLifecycleConfig">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteNotebookInstanceLifecycleConfigOutcome DeleteNotebookInstanceLifecycleConfig(const Model::DeleteNotebookInstanceLifecycleConfigRequest& request) const;


        /**
         * <p>Deletes a pipeline if there are no running instances of the pipeline. To
         * delete a pipeline, you must stop all running instances of the pipeline using the
         * <code>StopPipelineExecution</code> API. When you delete a pipeline, all
         * instances of the pipeline are deleted.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeletePipeline">AWS
         * API Reference</a></p>
         */
        virtual Model::DeletePipelineOutcome DeletePipeline(const Model::DeletePipelineRequest& request) const;


        /**
         * <p>Delete the specified project.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteProject">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteProjectOutcome DeleteProject(const Model::DeleteProjectRequest& request) const;


        /**
         * <p>Deletes the Studio Lifecycle Configuration. In order to delete the Lifecycle
         * Configuration, there must be no running apps using the Lifecycle Configuration.
         * You must also remove the Lifecycle Configuration from UserSettings in all
         * Domains and UserProfiles.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteStudioLifecycleConfig">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteStudioLifecycleConfigOutcome DeleteStudioLifecycleConfig(const Model::DeleteStudioLifecycleConfigRequest& request) const;


        /**
         * <p>Deletes the specified tags from an SageMaker resource.</p> <p>To list a
         * resource's tags, use the <code>ListTags</code> API. </p>  <p>When you call
         * this API to delete tags from a hyperparameter tuning job, the deleted tags are
         * not removed from training jobs that the hyperparameter tuning job launched
         * before you called this API.</p>   <p>When you call this API to
         * delete tags from a SageMaker Studio Domain or User Profile, the deleted tags are
         * not removed from Apps that the SageMaker Studio Domain or User Profile launched
         * before you called this API.</p> <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteTags">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteTagsOutcome DeleteTags(const Model::DeleteTagsRequest& request) const;


        /**
         * <p>Deletes the specified trial. All trial components that make up the trial must
         * be deleted first. Use the <a>DescribeTrialComponent</a> API to get the list of
         * trial components.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteTrial">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteTrialOutcome DeleteTrial(const Model::DeleteTrialRequest& request) const;


        /**
         * <p>Deletes the specified trial component. A trial component must be
         * disassociated from all trials before the trial component can be deleted. To
         * disassociate a trial component from a trial, call the
         * <a>DisassociateTrialComponent</a> API.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteTrialComponent">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteTrialComponentOutcome DeleteTrialComponent(const Model::DeleteTrialComponentRequest& request) const;


        /**
         * <p>Deletes a user profile. When a user profile is deleted, the user loses access
         * to their EFS volume, including data, notebooks, and other
         * artifacts.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteUserProfile">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteUserProfileOutcome DeleteUserProfile(const Model::DeleteUserProfileRequest& request) const;


        /**
         * <p>Use this operation to delete a workforce.</p> <p>If you want to create a new
         * workforce in an Amazon Web Services Region where a workforce already exists, use
         * this operation to delete the existing workforce and then use to create a new
         * workforce.</p>  <p>If a private workforce contains one or more work
         * teams, you must use the operation to delete all work teams before you delete the
         * workforce. If you try to delete a workforce that contains one or more work
         * teams, you will recieve a <code>ResourceInUse</code> error.</p>
         * <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteWorkforce">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteWorkforceOutcome DeleteWorkforce(const Model::DeleteWorkforceRequest& request) const;


        /**
         * <p>Deletes an existing work team. This operation can't be undone.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeleteWorkteam">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteWorkteamOutcome DeleteWorkteam(const Model::DeleteWorkteamRequest& request) const;


        /**
         * <p>Deregisters the specified devices. After you deregister a device, you will
         * need to re-register the devices.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DeregisterDevices">AWS
         * API Reference</a></p>
         */
        virtual Model::DeregisterDevicesOutcome DeregisterDevices(const Model::DeregisterDevicesRequest& request) const;


        /**
         * <p>Describes an action.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeAction">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeActionOutcome DescribeAction(const Model::DescribeActionRequest& request) const;


        /**
         * <p>Returns a description of the specified algorithm that is in your
         * account.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeAlgorithm">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeAlgorithmOutcome DescribeAlgorithm(const Model::DescribeAlgorithmRequest& request) const;


        /**
         * <p>Describes the app.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeApp">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeAppOutcome DescribeApp(const Model::DescribeAppRequest& request) const;


        /**
         * <p>Describes an AppImageConfig.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeAppImageConfig">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeAppImageConfigOutcome DescribeAppImageConfig(const Model::DescribeAppImageConfigRequest& request) const;


        /**
         * <p>Describes an artifact.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeArtifact">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeArtifactOutcome DescribeArtifact(const Model::DescribeArtifactRequest& request) const;


        /**
         * <p>Returns information about an Amazon SageMaker AutoML job.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeAutoMLJob">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeAutoMLJobOutcome DescribeAutoMLJob(const Model::DescribeAutoMLJobRequest& request) const;


        /**
         * <p>Gets details about the specified Git repository.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeCodeRepository">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeCodeRepositoryOutcome DescribeCodeRepository(const Model::DescribeCodeRepositoryRequest& request) const;


        /**
         * <p>Returns information about a model compilation job.</p> <p>To create a model
         * compilation job, use <a>CreateCompilationJob</a>. To get information about
         * multiple model compilation jobs, use <a>ListCompilationJobs</a>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeCompilationJob">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeCompilationJobOutcome DescribeCompilationJob(const Model::DescribeCompilationJobRequest& request) const;


        /**
         * <p>Describes a context.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeContext">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeContextOutcome DescribeContext(const Model::DescribeContextRequest& request) const;


        /**
         * <p>Gets the details of a data quality monitoring job definition.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeDataQualityJobDefinition">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeDataQualityJobDefinitionOutcome DescribeDataQualityJobDefinition(const Model::DescribeDataQualityJobDefinitionRequest& request) const;


        /**
         * <p>Describes the device.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeDevice">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeDeviceOutcome DescribeDevice(const Model::DescribeDeviceRequest& request) const;


        /**
         * <p>A description of the fleet the device belongs to.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeDeviceFleet">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeDeviceFleetOutcome DescribeDeviceFleet(const Model::DescribeDeviceFleetRequest& request) const;


        /**
         * <p>The description of the domain.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeDomain">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeDomainOutcome DescribeDomain(const Model::DescribeDomainRequest& request) const;


        /**
         * <p>Describes an edge deployment plan with deployment status per
         * stage.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeEdgeDeploymentPlan">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeEdgeDeploymentPlanOutcome DescribeEdgeDeploymentPlan(const Model::DescribeEdgeDeploymentPlanRequest& request) const;


        /**
         * <p>A description of edge packaging jobs.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeEdgePackagingJob">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeEdgePackagingJobOutcome DescribeEdgePackagingJob(const Model::DescribeEdgePackagingJobRequest& request) const;


        /**
         * <p>Returns the description of an endpoint.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeEndpoint">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeEndpointOutcome DescribeEndpoint(const Model::DescribeEndpointRequest& request) const;


        /**
         * <p>Returns the description of an endpoint configuration created using the
         * <code>CreateEndpointConfig</code> API.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeEndpointConfig">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeEndpointConfigOutcome DescribeEndpointConfig(const Model::DescribeEndpointConfigRequest& request) const;


        /**
         * <p>Provides a list of an experiment's properties.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeExperiment">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeExperimentOutcome DescribeExperiment(const Model::DescribeExperimentRequest& request) const;


        /**
         * <p>Use this operation to describe a <code>FeatureGroup</code>. The response
         * includes information on the creation time, <code>FeatureGroup</code> name, the
         * unique identifier for each <code>FeatureGroup</code>, and more.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeFeatureGroup">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeFeatureGroupOutcome DescribeFeatureGroup(const Model::DescribeFeatureGroupRequest& request) const;


        /**
         * <p>Shows the metadata for a feature within a feature group.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeFeatureMetadata">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeFeatureMetadataOutcome DescribeFeatureMetadata(const Model::DescribeFeatureMetadataRequest& request) const;


        /**
         * <p>Returns information about the specified flow definition.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeFlowDefinition">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeFlowDefinitionOutcome DescribeFlowDefinition(const Model::DescribeFlowDefinitionRequest& request) const;


        /**
         * <p>Returns information about the requested human task user interface (worker
         * task template).</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeHumanTaskUi">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeHumanTaskUiOutcome DescribeHumanTaskUi(const Model::DescribeHumanTaskUiRequest& request) const;


        /**
         * <p>Gets a description of a hyperparameter tuning job.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeHyperParameterTuningJob">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeHyperParameterTuningJobOutcome DescribeHyperParameterTuningJob(const Model::DescribeHyperParameterTuningJobRequest& request) const;


        /**
         * <p>Describes a SageMaker image.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeImage">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeImageOutcome DescribeImage(const Model::DescribeImageRequest& request) const;


        /**
         * <p>Describes a version of a SageMaker image.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeImageVersion">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeImageVersionOutcome DescribeImageVersion(const Model::DescribeImageVersionRequest& request) const;


        /**
         * <p>Provides the results of the Inference Recommender job. One or more
         * recommendation jobs are returned.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeInferenceRecommendationsJob">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeInferenceRecommendationsJobOutcome DescribeInferenceRecommendationsJob(const Model::DescribeInferenceRecommendationsJobRequest& request) const;


        /**
         * <p>Gets information about a labeling job.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeLabelingJob">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeLabelingJobOutcome DescribeLabelingJob(const Model::DescribeLabelingJobRequest& request) const;


        /**
         * <p>Provides a list of properties for the requested lineage group. For more
         * information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/xaccount-lineage-tracking.html">
         * Cross-Account Lineage Tracking </a> in the <i>Amazon SageMaker Developer
         * Guide</i>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeLineageGroup">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeLineageGroupOutcome DescribeLineageGroup(const Model::DescribeLineageGroupRequest& request) const;


        /**
         * <p>Describes a model that you created using the <code>CreateModel</code>
         * API.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeModel">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeModelOutcome DescribeModel(const Model::DescribeModelRequest& request) const;


        /**
         * <p>Returns a description of a model bias job definition.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeModelBiasJobDefinition">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeModelBiasJobDefinitionOutcome DescribeModelBiasJobDefinition(const Model::DescribeModelBiasJobDefinitionRequest& request) const;


        /**
         * <p>Returns a description of a model explainability job definition.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeModelExplainabilityJobDefinition">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeModelExplainabilityJobDefinitionOutcome DescribeModelExplainabilityJobDefinition(const Model::DescribeModelExplainabilityJobDefinitionRequest& request) const;


        /**
         * <p>Returns a description of the specified model package, which is used to create
         * SageMaker models or list them on Amazon Web Services Marketplace.</p> <p>To
         * create models in SageMaker, buyers can subscribe to model packages listed on
         * Amazon Web Services Marketplace.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeModelPackage">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeModelPackageOutcome DescribeModelPackage(const Model::DescribeModelPackageRequest& request) const;


        /**
         * <p>Gets a description for the specified model group.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeModelPackageGroup">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeModelPackageGroupOutcome DescribeModelPackageGroup(const Model::DescribeModelPackageGroupRequest& request) const;


        /**
         * <p>Returns a description of a model quality job definition.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeModelQualityJobDefinition">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeModelQualityJobDefinitionOutcome DescribeModelQualityJobDefinition(const Model::DescribeModelQualityJobDefinitionRequest& request) const;


        /**
         * <p>Describes the schedule for a monitoring job.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeMonitoringSchedule">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeMonitoringScheduleOutcome DescribeMonitoringSchedule(const Model::DescribeMonitoringScheduleRequest& request) const;


        /**
         * <p>Returns information about a notebook instance.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeNotebookInstance">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeNotebookInstanceOutcome DescribeNotebookInstance(const Model::DescribeNotebookInstanceRequest& request) const;


        /**
         * <p>Returns a description of a notebook instance lifecycle configuration.</p>
         * <p>For information about notebook instance lifestyle configurations, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/notebook-lifecycle-config.html">Step
         * 2.1: (Optional) Customize a Notebook Instance</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeNotebookInstanceLifecycleConfig">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeNotebookInstanceLifecycleConfigOutcome DescribeNotebookInstanceLifecycleConfig(const Model::DescribeNotebookInstanceLifecycleConfigRequest& request) const;


        /**
         * <p>Describes the details of a pipeline.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribePipeline">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribePipelineOutcome DescribePipeline(const Model::DescribePipelineRequest& request) const;


        /**
         * <p>Describes the details of an execution's pipeline definition.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribePipelineDefinitionForExecution">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribePipelineDefinitionForExecutionOutcome DescribePipelineDefinitionForExecution(const Model::DescribePipelineDefinitionForExecutionRequest& request) const;


        /**
         * <p>Describes the details of a pipeline execution.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribePipelineExecution">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribePipelineExecutionOutcome DescribePipelineExecution(const Model::DescribePipelineExecutionRequest& request) const;


        /**
         * <p>Returns a description of a processing job.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeProcessingJob">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeProcessingJobOutcome DescribeProcessingJob(const Model::DescribeProcessingJobRequest& request) const;


        /**
         * <p>Describes the details of a project.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeProject">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeProjectOutcome DescribeProject(const Model::DescribeProjectRequest& request) const;


        /**
         * <p>Describes the Studio Lifecycle Configuration.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeStudioLifecycleConfig">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeStudioLifecycleConfigOutcome DescribeStudioLifecycleConfig(const Model::DescribeStudioLifecycleConfigRequest& request) const;


        /**
         * <p>Gets information about a work team provided by a vendor. It returns details
         * about the subscription with a vendor in the Amazon Web Services
         * Marketplace.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeSubscribedWorkteam">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeSubscribedWorkteamOutcome DescribeSubscribedWorkteam(const Model::DescribeSubscribedWorkteamRequest& request) const;


        /**
         * <p>Returns information about a training job. </p> <p>Some of the attributes
         * below only appear if the training job successfully starts. If the training job
         * fails, <code>TrainingJobStatus</code> is <code>Failed</code> and, depending on
         * the <code>FailureReason</code>, attributes like <code>TrainingStartTime</code>,
         * <code>TrainingTimeInSeconds</code>, <code>TrainingEndTime</code>, and
         * <code>BillableTimeInSeconds</code> may not be present in the
         * response.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeTrainingJob">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeTrainingJobOutcome DescribeTrainingJob(const Model::DescribeTrainingJobRequest& request) const;


        /**
         * <p>Returns information about a transform job.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeTransformJob">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeTransformJobOutcome DescribeTransformJob(const Model::DescribeTransformJobRequest& request) const;


        /**
         * <p>Provides a list of a trial's properties.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeTrial">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeTrialOutcome DescribeTrial(const Model::DescribeTrialRequest& request) const;


        /**
         * <p>Provides a list of a trials component's properties.</p><p><h3>See Also:</h3> 
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeTrialComponent">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeTrialComponentOutcome DescribeTrialComponent(const Model::DescribeTrialComponentRequest& request) const;


        /**
         * <p>Describes a user profile. For more information, see
         * <code>CreateUserProfile</code>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeUserProfile">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeUserProfileOutcome DescribeUserProfile(const Model::DescribeUserProfileRequest& request) const;


        /**
         * <p>Lists private workforce information, including workforce name, Amazon
         * Resource Name (ARN), and, if applicable, allowed IP address ranges (<a
         * href="https://docs.aws.amazon.com/vpc/latest/userguide/VPC_Subnets.html">CIDRs</a>).
         * Allowable IP address ranges are the IP addresses that workers can use to access
         * tasks. </p>  <p>This operation applies only to private
         * workforces.</p> <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeWorkforce">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeWorkforceOutcome DescribeWorkforce(const Model::DescribeWorkforceRequest& request) const;


        /**
         * <p>Gets information about a specific work team. You can see information such as
         * the create date, the last updated date, membership information, and the work
         * team's Amazon Resource Name (ARN).</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DescribeWorkteam">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeWorkteamOutcome DescribeWorkteam(const Model::DescribeWorkteamRequest& request) const;


        /**
         * <p>Disables using Service Catalog in SageMaker. Service Catalog is used to
         * create SageMaker projects.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DisableSagemakerServicecatalogPortfolio">AWS
         * API Reference</a></p>
         */
        virtual Model::DisableSagemakerServicecatalogPortfolioOutcome DisableSagemakerServicecatalogPortfolio(const Model::DisableSagemakerServicecatalogPortfolioRequest& request) const;


        /**
         * <p>Disassociates a trial component from a trial. This doesn't effect other
         * trials the component is associated with. Before you can delete a component, you
         * must disassociate the component from all trials it is associated with. To
         * associate a trial component with a trial, call the
         * <a>AssociateTrialComponent</a> API.</p> <p>To get a list of the trials a
         * component is associated with, use the <a>Search</a> API. Specify
         * <code>ExperimentTrialComponent</code> for the <code>Resource</code> parameter.
         * The list appears in the response under
         * <code>Results.TrialComponent.Parents</code>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/DisassociateTrialComponent">AWS
         * API Reference</a></p>
         */
        virtual Model::DisassociateTrialComponentOutcome DisassociateTrialComponent(const Model::DisassociateTrialComponentRequest& request) const;


        /**
         * <p>Enables using Service Catalog in SageMaker. Service Catalog is used to create
         * SageMaker projects.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/EnableSagemakerServicecatalogPortfolio">AWS
         * API Reference</a></p>
         */
        virtual Model::EnableSagemakerServicecatalogPortfolioOutcome EnableSagemakerServicecatalogPortfolio(const Model::EnableSagemakerServicecatalogPortfolioRequest& request) const;


        /**
         * <p>Describes a fleet.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/GetDeviceFleetReport">AWS
         * API Reference</a></p>
         */
        virtual Model::GetDeviceFleetReportOutcome GetDeviceFleetReport(const Model::GetDeviceFleetReportRequest& request) const;


        /**
         * <p>The resource policy for the lineage group.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/GetLineageGroupPolicy">AWS
         * API Reference</a></p>
         */
        virtual Model::GetLineageGroupPolicyOutcome GetLineageGroupPolicy(const Model::GetLineageGroupPolicyRequest& request) const;


        /**
         * <p>Gets a resource policy that manages access for a model group. For information
         * about resource policies, see <a
         * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/access_policies_identity-vs-resource.html">Identity-based
         * policies and resource-based policies</a> in the <i>Amazon Web Services Identity
         * and Access Management User Guide.</i>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/GetModelPackageGroupPolicy">AWS
         * API Reference</a></p>
         */
        virtual Model::GetModelPackageGroupPolicyOutcome GetModelPackageGroupPolicy(const Model::GetModelPackageGroupPolicyRequest& request) const;


        /**
         * <p>Gets the status of Service Catalog in SageMaker. Service Catalog is used to
         * create SageMaker projects.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/GetSagemakerServicecatalogPortfolioStatus">AWS
         * API Reference</a></p>
         */
        virtual Model::GetSagemakerServicecatalogPortfolioStatusOutcome GetSagemakerServicecatalogPortfolioStatus(const Model::GetSagemakerServicecatalogPortfolioStatusRequest& request) const;


        /**
         * <p>An auto-complete API for the search functionality in the Amazon SageMaker
         * console. It returns suggestions of possible matches for the property name to use
         * in <code>Search</code> queries. Provides suggestions for
         * <code>HyperParameters</code>, <code>Tags</code>, and
         * <code>Metrics</code>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/GetSearchSuggestions">AWS
         * API Reference</a></p>
         */
        virtual Model::GetSearchSuggestionsOutcome GetSearchSuggestions(const Model::GetSearchSuggestionsRequest& request) const;


        /**
         * <p>Lists the actions in your account and their properties.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListActions">AWS
         * API Reference</a></p>
         */
        virtual Model::ListActionsOutcome ListActions(const Model::ListActionsRequest& request) const;


        /**
         * <p>Lists the machine learning algorithms that have been created.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListAlgorithms">AWS
         * API Reference</a></p>
         */
        virtual Model::ListAlgorithmsOutcome ListAlgorithms(const Model::ListAlgorithmsRequest& request) const;


        /**
         * <p>Lists the AppImageConfigs in your account and their properties. The list can
         * be filtered by creation time or modified time, and whether the AppImageConfig
         * name contains a specified string.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListAppImageConfigs">AWS
         * API Reference</a></p>
         */
        virtual Model::ListAppImageConfigsOutcome ListAppImageConfigs(const Model::ListAppImageConfigsRequest& request) const;


        /**
         * <p>Lists apps.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListApps">AWS
         * API Reference</a></p>
         */
        virtual Model::ListAppsOutcome ListApps(const Model::ListAppsRequest& request) const;


        /**
         * <p>Lists the artifacts in your account and their properties.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListArtifacts">AWS
         * API Reference</a></p>
         */
        virtual Model::ListArtifactsOutcome ListArtifacts(const Model::ListArtifactsRequest& request) const;


        /**
         * <p>Lists the associations in your account and their properties.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListAssociations">AWS
         * API Reference</a></p>
         */
        virtual Model::ListAssociationsOutcome ListAssociations(const Model::ListAssociationsRequest& request) const;


        /**
         * <p>Request a list of jobs.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListAutoMLJobs">AWS
         * API Reference</a></p>
         */
        virtual Model::ListAutoMLJobsOutcome ListAutoMLJobs(const Model::ListAutoMLJobsRequest& request) const;


        /**
         * <p>List the candidates created for the job.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListCandidatesForAutoMLJob">AWS
         * API Reference</a></p>
         */
        virtual Model::ListCandidatesForAutoMLJobOutcome ListCandidatesForAutoMLJob(const Model::ListCandidatesForAutoMLJobRequest& request) const;


        /**
         * <p>Gets a list of the Git repositories in your account.</p><p><h3>See Also:</h3>
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListCodeRepositories">AWS
         * API Reference</a></p>
         */
        virtual Model::ListCodeRepositoriesOutcome ListCodeRepositories(const Model::ListCodeRepositoriesRequest& request) const;


        /**
         * <p>Lists model compilation jobs that satisfy various filters.</p> <p>To create a
         * model compilation job, use <a>CreateCompilationJob</a>. To get information about
         * a particular model compilation job you have created, use
         * <a>DescribeCompilationJob</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListCompilationJobs">AWS
         * API Reference</a></p>
         */
        virtual Model::ListCompilationJobsOutcome ListCompilationJobs(const Model::ListCompilationJobsRequest& request) const;


        /**
         * <p>Lists the contexts in your account and their properties.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListContexts">AWS
         * API Reference</a></p>
         */
        virtual Model::ListContextsOutcome ListContexts(const Model::ListContextsRequest& request) const;


        /**
         * <p>Lists the data quality job definitions in your account.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListDataQualityJobDefinitions">AWS
         * API Reference</a></p>
         */
        virtual Model::ListDataQualityJobDefinitionsOutcome ListDataQualityJobDefinitions(const Model::ListDataQualityJobDefinitionsRequest& request) const;


        /**
         * <p>Returns a list of devices in the fleet.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListDeviceFleets">AWS
         * API Reference</a></p>
         */
        virtual Model::ListDeviceFleetsOutcome ListDeviceFleets(const Model::ListDeviceFleetsRequest& request) const;


        /**
         * <p>A list of devices.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListDevices">AWS
         * API Reference</a></p>
         */
        virtual Model::ListDevicesOutcome ListDevices(const Model::ListDevicesRequest& request) const;


        /**
         * <p>Lists the domains.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListDomains">AWS
         * API Reference</a></p>
         */
        virtual Model::ListDomainsOutcome ListDomains(const Model::ListDomainsRequest& request) const;


        /**
         * <p>Lists all edge deployment plans.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListEdgeDeploymentPlans">AWS
         * API Reference</a></p>
         */
        virtual Model::ListEdgeDeploymentPlansOutcome ListEdgeDeploymentPlans(const Model::ListEdgeDeploymentPlansRequest& request) const;


        /**
         * <p>Returns a list of edge packaging jobs.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListEdgePackagingJobs">AWS
         * API Reference</a></p>
         */
        virtual Model::ListEdgePackagingJobsOutcome ListEdgePackagingJobs(const Model::ListEdgePackagingJobsRequest& request) const;


        /**
         * <p>Lists endpoint configurations.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListEndpointConfigs">AWS
         * API Reference</a></p>
         */
        virtual Model::ListEndpointConfigsOutcome ListEndpointConfigs(const Model::ListEndpointConfigsRequest& request) const;


        /**
         * <p>Lists endpoints.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListEndpoints">AWS
         * API Reference</a></p>
         */
        virtual Model::ListEndpointsOutcome ListEndpoints(const Model::ListEndpointsRequest& request) const;


        /**
         * <p>Lists all the experiments in your account. The list can be filtered to show
         * only experiments that were created in a specific time range. The list can be
         * sorted by experiment name or creation time.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListExperiments">AWS
         * API Reference</a></p>
         */
        virtual Model::ListExperimentsOutcome ListExperiments(const Model::ListExperimentsRequest& request) const;


        /**
         * <p>List <code>FeatureGroup</code>s based on given filter and
         * order.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListFeatureGroups">AWS
         * API Reference</a></p>
         */
        virtual Model::ListFeatureGroupsOutcome ListFeatureGroups(const Model::ListFeatureGroupsRequest& request) const;


        /**
         * <p>Returns information about the flow definitions in your account.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListFlowDefinitions">AWS
         * API Reference</a></p>
         */
        virtual Model::ListFlowDefinitionsOutcome ListFlowDefinitions(const Model::ListFlowDefinitionsRequest& request) const;


        /**
         * <p>Returns information about the human task user interfaces in your
         * account.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListHumanTaskUis">AWS
         * API Reference</a></p>
         */
        virtual Model::ListHumanTaskUisOutcome ListHumanTaskUis(const Model::ListHumanTaskUisRequest& request) const;


        /**
         * <p>Gets a list of <a>HyperParameterTuningJobSummary</a> objects that describe
         * the hyperparameter tuning jobs launched in your account.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListHyperParameterTuningJobs">AWS
         * API Reference</a></p>
         */
        virtual Model::ListHyperParameterTuningJobsOutcome ListHyperParameterTuningJobs(const Model::ListHyperParameterTuningJobsRequest& request) const;


        /**
         * <p>Lists the versions of a specified image and their properties. The list can be
         * filtered by creation time or modified time.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListImageVersions">AWS
         * API Reference</a></p>
         */
        virtual Model::ListImageVersionsOutcome ListImageVersions(const Model::ListImageVersionsRequest& request) const;


        /**
         * <p>Lists the images in your account and their properties. The list can be
         * filtered by creation time or modified time, and whether the image name contains
         * a specified string.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListImages">AWS
         * API Reference</a></p>
         */
        virtual Model::ListImagesOutcome ListImages(const Model::ListImagesRequest& request) const;


        /**
         * <p>Returns a list of the subtasks for an Inference Recommender job.</p> <p>The
         * supported subtasks are benchmarks, which evaluate the performance of your model
         * on different instance types.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListInferenceRecommendationsJobSteps">AWS
         * API Reference</a></p>
         */
        virtual Model::ListInferenceRecommendationsJobStepsOutcome ListInferenceRecommendationsJobSteps(const Model::ListInferenceRecommendationsJobStepsRequest& request) const;


        /**
         * <p>Lists recommendation jobs that satisfy various filters.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListInferenceRecommendationsJobs">AWS
         * API Reference</a></p>
         */
        virtual Model::ListInferenceRecommendationsJobsOutcome ListInferenceRecommendationsJobs(const Model::ListInferenceRecommendationsJobsRequest& request) const;


        /**
         * <p>Gets a list of labeling jobs.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListLabelingJobs">AWS
         * API Reference</a></p>
         */
        virtual Model::ListLabelingJobsOutcome ListLabelingJobs(const Model::ListLabelingJobsRequest& request) const;


        /**
         * <p>Gets a list of labeling jobs assigned to a specified work team.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListLabelingJobsForWorkteam">AWS
         * API Reference</a></p>
         */
        virtual Model::ListLabelingJobsForWorkteamOutcome ListLabelingJobsForWorkteam(const Model::ListLabelingJobsForWorkteamRequest& request) const;


        /**
         * <p>A list of lineage groups shared with your Amazon Web Services account. For
         * more information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/xaccount-lineage-tracking.html">
         * Cross-Account Lineage Tracking </a> in the <i>Amazon SageMaker Developer
         * Guide</i>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListLineageGroups">AWS
         * API Reference</a></p>
         */
        virtual Model::ListLineageGroupsOutcome ListLineageGroups(const Model::ListLineageGroupsRequest& request) const;


        /**
         * <p>Lists model bias jobs definitions that satisfy various filters.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListModelBiasJobDefinitions">AWS
         * API Reference</a></p>
         */
        virtual Model::ListModelBiasJobDefinitionsOutcome ListModelBiasJobDefinitions(const Model::ListModelBiasJobDefinitionsRequest& request) const;


        /**
         * <p>Lists model explainability job definitions that satisfy various
         * filters.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListModelExplainabilityJobDefinitions">AWS
         * API Reference</a></p>
         */
        virtual Model::ListModelExplainabilityJobDefinitionsOutcome ListModelExplainabilityJobDefinitions(const Model::ListModelExplainabilityJobDefinitionsRequest& request) const;


        /**
         * <p>Lists the domain, framework, task, and model name of standard machine
         * learning models found in common model zoos.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListModelMetadata">AWS
         * API Reference</a></p>
         */
        virtual Model::ListModelMetadataOutcome ListModelMetadata(const Model::ListModelMetadataRequest& request) const;


        /**
         * <p>Gets a list of the model groups in your Amazon Web Services
         * account.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListModelPackageGroups">AWS
         * API Reference</a></p>
         */
        virtual Model::ListModelPackageGroupsOutcome ListModelPackageGroups(const Model::ListModelPackageGroupsRequest& request) const;


        /**
         * <p>Lists the model packages that have been created.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListModelPackages">AWS
         * API Reference</a></p>
         */
        virtual Model::ListModelPackagesOutcome ListModelPackages(const Model::ListModelPackagesRequest& request) const;


        /**
         * <p>Gets a list of model quality monitoring job definitions in your
         * account.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListModelQualityJobDefinitions">AWS
         * API Reference</a></p>
         */
        virtual Model::ListModelQualityJobDefinitionsOutcome ListModelQualityJobDefinitions(const Model::ListModelQualityJobDefinitionsRequest& request) const;


        /**
         * <p>Lists models created with the <code>CreateModel</code> API.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListModels">AWS
         * API Reference</a></p>
         */
        virtual Model::ListModelsOutcome ListModels(const Model::ListModelsRequest& request) const;


        /**
         * <p>Returns list of all monitoring job executions.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListMonitoringExecutions">AWS
         * API Reference</a></p>
         */
        virtual Model::ListMonitoringExecutionsOutcome ListMonitoringExecutions(const Model::ListMonitoringExecutionsRequest& request) const;


        /**
         * <p>Returns list of all monitoring schedules.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListMonitoringSchedules">AWS
         * API Reference</a></p>
         */
        virtual Model::ListMonitoringSchedulesOutcome ListMonitoringSchedules(const Model::ListMonitoringSchedulesRequest& request) const;


        /**
         * <p>Lists notebook instance lifestyle configurations created with the
         * <a>CreateNotebookInstanceLifecycleConfig</a> API.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListNotebookInstanceLifecycleConfigs">AWS
         * API Reference</a></p>
         */
        virtual Model::ListNotebookInstanceLifecycleConfigsOutcome ListNotebookInstanceLifecycleConfigs(const Model::ListNotebookInstanceLifecycleConfigsRequest& request) const;


        /**
         * <p>Returns a list of the SageMaker notebook instances in the requester's account
         * in an Amazon Web Services Region. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListNotebookInstances">AWS
         * API Reference</a></p>
         */
        virtual Model::ListNotebookInstancesOutcome ListNotebookInstances(const Model::ListNotebookInstancesRequest& request) const;


        /**
         * <p>Gets a list of <code>PipeLineExecutionStep</code> objects.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListPipelineExecutionSteps">AWS
         * API Reference</a></p>
         */
        virtual Model::ListPipelineExecutionStepsOutcome ListPipelineExecutionSteps(const Model::ListPipelineExecutionStepsRequest& request) const;


        /**
         * <p>Gets a list of the pipeline executions.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListPipelineExecutions">AWS
         * API Reference</a></p>
         */
        virtual Model::ListPipelineExecutionsOutcome ListPipelineExecutions(const Model::ListPipelineExecutionsRequest& request) const;


        /**
         * <p>Gets a list of parameters for a pipeline execution.</p><p><h3>See Also:</h3> 
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListPipelineParametersForExecution">AWS
         * API Reference</a></p>
         */
        virtual Model::ListPipelineParametersForExecutionOutcome ListPipelineParametersForExecution(const Model::ListPipelineParametersForExecutionRequest& request) const;


        /**
         * <p>Gets a list of pipelines.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListPipelines">AWS
         * API Reference</a></p>
         */
        virtual Model::ListPipelinesOutcome ListPipelines(const Model::ListPipelinesRequest& request) const;


        /**
         * <p>Lists processing jobs that satisfy various filters.</p><p><h3>See Also:</h3> 
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListProcessingJobs">AWS
         * API Reference</a></p>
         */
        virtual Model::ListProcessingJobsOutcome ListProcessingJobs(const Model::ListProcessingJobsRequest& request) const;


        /**
         * <p>Gets a list of the projects in an Amazon Web Services account.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListProjects">AWS
         * API Reference</a></p>
         */
        virtual Model::ListProjectsOutcome ListProjects(const Model::ListProjectsRequest& request) const;


        /**
         * <p>Lists devices allocated to the stage, containing detailed device information
         * and deployment status.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListStageDevices">AWS
         * API Reference</a></p>
         */
        virtual Model::ListStageDevicesOutcome ListStageDevices(const Model::ListStageDevicesRequest& request) const;


        /**
         * <p>Lists the Studio Lifecycle Configurations in your Amazon Web Services
         * Account.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListStudioLifecycleConfigs">AWS
         * API Reference</a></p>
         */
        virtual Model::ListStudioLifecycleConfigsOutcome ListStudioLifecycleConfigs(const Model::ListStudioLifecycleConfigsRequest& request) const;


        /**
         * <p>Gets a list of the work teams that you are subscribed to in the Amazon Web
         * Services Marketplace. The list may be empty if no work team satisfies the filter
         * specified in the <code>NameContains</code> parameter.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListSubscribedWorkteams">AWS
         * API Reference</a></p>
         */
        virtual Model::ListSubscribedWorkteamsOutcome ListSubscribedWorkteams(const Model::ListSubscribedWorkteamsRequest& request) const;


        /**
         * <p>Returns the tags for the specified SageMaker resource.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListTags">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTagsOutcome ListTags(const Model::ListTagsRequest& request) const;


        /**
         * <p>Lists training jobs.</p>  <p>When <code>StatusEquals</code> and
         * <code>MaxResults</code> are set at the same time, the <code>MaxResults</code>
         * number of training jobs are first retrieved ignoring the
         * <code>StatusEquals</code> parameter and then they are filtered by the
         * <code>StatusEquals</code> parameter, which is returned as a response.</p> <p>For
         * example, if <code>ListTrainingJobs</code> is invoked with the following
         * parameters:</p> <p> <code>{ ... MaxResults: 100, StatusEquals: InProgress ...
         * }</code> </p> <p>First, 100 trainings jobs with any status, including those
         * other than <code>InProgress</code>, are selected (sorted according to the
         * creation time, from the most current to the oldest). Next, those with a status
         * of <code>InProgress</code> are returned.</p> <p>You can quickly test the API
         * using the following Amazon Web Services CLI code.</p> <p> <code>aws sagemaker
         * list-training-jobs --max-results 100 --status-equals InProgress</code> </p>
         * <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListTrainingJobs">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTrainingJobsOutcome ListTrainingJobs(const Model::ListTrainingJobsRequest& request) const;


        /**
         * <p>Gets a list of <a>TrainingJobSummary</a> objects that describe the training
         * jobs that a hyperparameter tuning job launched.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListTrainingJobsForHyperParameterTuningJob">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTrainingJobsForHyperParameterTuningJobOutcome ListTrainingJobsForHyperParameterTuningJob(const Model::ListTrainingJobsForHyperParameterTuningJobRequest& request) const;


        /**
         * <p>Lists transform jobs.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListTransformJobs">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTransformJobsOutcome ListTransformJobs(const Model::ListTransformJobsRequest& request) const;


        /**
         * <p>Lists the trial components in your account. You can sort the list by trial
         * component name or creation time. You can filter the list to show only components
         * that were created in a specific time range. You can also filter on one of the
         * following:</p> <ul> <li> <p> <code>ExperimentName</code> </p> </li> <li> <p>
         * <code>SourceArn</code> </p> </li> <li> <p> <code>TrialName</code> </p> </li>
         * </ul><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListTrialComponents">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTrialComponentsOutcome ListTrialComponents(const Model::ListTrialComponentsRequest& request) const;


        /**
         * <p>Lists the trials in your account. Specify an experiment name to limit the
         * list to the trials that are part of that experiment. Specify a trial component
         * name to limit the list to the trials that associated with that trial component.
         * The list can be filtered to show only trials that were created in a specific
         * time range. The list can be sorted by trial name or creation time.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListTrials">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTrialsOutcome ListTrials(const Model::ListTrialsRequest& request) const;


        /**
         * <p>Lists user profiles.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListUserProfiles">AWS
         * API Reference</a></p>
         */
        virtual Model::ListUserProfilesOutcome ListUserProfiles(const Model::ListUserProfilesRequest& request) const;


        /**
         * <p>Use this operation to list all private and vendor workforces in an Amazon Web
         * Services Region. Note that you can only have one private workforce per Amazon
         * Web Services Region.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListWorkforces">AWS
         * API Reference</a></p>
         */
        virtual Model::ListWorkforcesOutcome ListWorkforces(const Model::ListWorkforcesRequest& request) const;


        /**
         * <p>Gets a list of private work teams that you have defined in a region. The list
         * may be empty if no work team satisfies the filter specified in the
         * <code>NameContains</code> parameter.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/ListWorkteams">AWS
         * API Reference</a></p>
         */
        virtual Model::ListWorkteamsOutcome ListWorkteams(const Model::ListWorkteamsRequest& request) const;


        /**
         * <p>Adds a resouce policy to control access to a model group. For information
         * about resoure policies, see <a
         * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/access_policies_identity-vs-resource.html">Identity-based
         * policies and resource-based policies</a> in the <i>Amazon Web Services Identity
         * and Access Management User Guide.</i>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/PutModelPackageGroupPolicy">AWS
         * API Reference</a></p>
         */
        virtual Model::PutModelPackageGroupPolicyOutcome PutModelPackageGroupPolicy(const Model::PutModelPackageGroupPolicyRequest& request) const;


        /**
         * <p>Use this action to inspect your lineage and discover relationships between
         * entities. For more information, see <a
         * href="https://docs.aws.amazon.com/sagemaker/latest/dg/querying-lineage-entities.html">
         * Querying Lineage Entities</a> in the <i>Amazon SageMaker Developer
         * Guide</i>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/QueryLineage">AWS
         * API Reference</a></p>
         */
        virtual Model::QueryLineageOutcome QueryLineage(const Model::QueryLineageRequest& request) const;


        /**
         * <p>Register devices.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/RegisterDevices">AWS
         * API Reference</a></p>
         */
        virtual Model::RegisterDevicesOutcome RegisterDevices(const Model::RegisterDevicesRequest& request) const;


        /**
         * <p>Renders the UI template so that you can preview the worker's experience.
         * </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/RenderUiTemplate">AWS
         * API Reference</a></p>
         */
        virtual Model::RenderUiTemplateOutcome RenderUiTemplate(const Model::RenderUiTemplateRequest& request) const;


        /**
         * <p>Retry the execution of the pipeline.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/RetryPipelineExecution">AWS
         * API Reference</a></p>
         */
        virtual Model::RetryPipelineExecutionOutcome RetryPipelineExecution(const Model::RetryPipelineExecutionRequest& request) const;


        /**
         * <p>Finds Amazon SageMaker resources that match a search query. Matching
         * resources are returned as a list of <code>SearchRecord</code> objects in the
         * response. You can sort the search results by any resource property in a
         * ascending or descending order.</p> <p>You can query against the following value
         * types: numeric, text, Boolean, and timestamp.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/Search">AWS
         * API Reference</a></p>
         */
        virtual Model::SearchOutcome Search(const Model::SearchRequest& request) const;


        /**
         * <p>Notifies the pipeline that the execution of a callback step failed, along
         * with a message describing why. When a callback step is run, the pipeline
         * generates a callback token and includes the token in a message sent to Amazon
         * Simple Queue Service (Amazon SQS).</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/SendPipelineExecutionStepFailure">AWS
         * API Reference</a></p>
         */
        virtual Model::SendPipelineExecutionStepFailureOutcome SendPipelineExecutionStepFailure(const Model::SendPipelineExecutionStepFailureRequest& request) const;


        /**
         * <p>Notifies the pipeline that the execution of a callback step succeeded and
         * provides a list of the step's output parameters. When a callback step is run,
         * the pipeline generates a callback token and includes the token in a message sent
         * to Amazon Simple Queue Service (Amazon SQS).</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/SendPipelineExecutionStepSuccess">AWS
         * API Reference</a></p>
         */
        virtual Model::SendPipelineExecutionStepSuccessOutcome SendPipelineExecutionStepSuccess(const Model::SendPipelineExecutionStepSuccessRequest& request) const;


        /**
         * <p>Starts a stage in an edge deployment plan.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StartEdgeDeploymentStage">AWS
         * API Reference</a></p>
         */
        virtual Model::StartEdgeDeploymentStageOutcome StartEdgeDeploymentStage(const Model::StartEdgeDeploymentStageRequest& request) const;


        /**
         * <p>Starts a previously stopped monitoring schedule.</p>  <p>By default,
         * when you successfully create a new schedule, the status of a monitoring schedule
         * is <code>scheduled</code>.</p> <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StartMonitoringSchedule">AWS
         * API Reference</a></p>
         */
        virtual Model::StartMonitoringScheduleOutcome StartMonitoringSchedule(const Model::StartMonitoringScheduleRequest& request) const;


        /**
         * <p>Launches an ML compute instance with the latest version of the libraries and
         * attaches your ML storage volume. After configuring the notebook instance,
         * SageMaker sets the notebook instance status to <code>InService</code>. A
         * notebook instance's status must be <code>InService</code> before you can connect
         * to your Jupyter notebook. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StartNotebookInstance">AWS
         * API Reference</a></p>
         */
        virtual Model::StartNotebookInstanceOutcome StartNotebookInstance(const Model::StartNotebookInstanceRequest& request) const;


        /**
         * <p>Starts a pipeline execution.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StartPipelineExecution">AWS
         * API Reference</a></p>
         */
        virtual Model::StartPipelineExecutionOutcome StartPipelineExecution(const Model::StartPipelineExecutionRequest& request) const;


        /**
         * <p>A method for forcing the termination of a running job.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StopAutoMLJob">AWS
         * API Reference</a></p>
         */
        virtual Model::StopAutoMLJobOutcome StopAutoMLJob(const Model::StopAutoMLJobRequest& request) const;


        /**
         * <p>Stops a model compilation job.</p> <p> To stop a job, Amazon SageMaker sends
         * the algorithm the SIGTERM signal. This gracefully shuts the job down. If the job
         * hasn't stopped, it sends the SIGKILL signal.</p> <p>When it receives a
         * <code>StopCompilationJob</code> request, Amazon SageMaker changes the
         * <a>CompilationJobSummary$CompilationJobStatus</a> of the job to
         * <code>Stopping</code>. After Amazon SageMaker stops the job, it sets the
         * <a>CompilationJobSummary$CompilationJobStatus</a> to <code>Stopped</code>.
         * </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StopCompilationJob">AWS
         * API Reference</a></p>
         */
        virtual Model::StopCompilationJobOutcome StopCompilationJob(const Model::StopCompilationJobRequest& request) const;


        /**
         * <p>Stops a stage in an edge deployment plan.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StopEdgeDeploymentStage">AWS
         * API Reference</a></p>
         */
        virtual Model::StopEdgeDeploymentStageOutcome StopEdgeDeploymentStage(const Model::StopEdgeDeploymentStageRequest& request) const;


        /**
         * <p>Request to stop an edge packaging job.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StopEdgePackagingJob">AWS
         * API Reference</a></p>
         */
        virtual Model::StopEdgePackagingJobOutcome StopEdgePackagingJob(const Model::StopEdgePackagingJobRequest& request) const;


        /**
         * <p>Stops a running hyperparameter tuning job and all running training jobs that
         * the tuning job launched.</p> <p>All model artifacts output from the training
         * jobs are stored in Amazon Simple Storage Service (Amazon S3). All data that the
         * training jobs write to Amazon CloudWatch Logs are still available in CloudWatch.
         * After the tuning job moves to the <code>Stopped</code> state, it releases all
         * reserved resources for the tuning job.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StopHyperParameterTuningJob">AWS
         * API Reference</a></p>
         */
        virtual Model::StopHyperParameterTuningJobOutcome StopHyperParameterTuningJob(const Model::StopHyperParameterTuningJobRequest& request) const;


        /**
         * <p>Stops an Inference Recommender job.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StopInferenceRecommendationsJob">AWS
         * API Reference</a></p>
         */
        virtual Model::StopInferenceRecommendationsJobOutcome StopInferenceRecommendationsJob(const Model::StopInferenceRecommendationsJobRequest& request) const;


        /**
         * <p>Stops a running labeling job. A job that is stopped cannot be restarted. Any
         * results obtained before the job is stopped are placed in the Amazon S3 output
         * bucket.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StopLabelingJob">AWS
         * API Reference</a></p>
         */
        virtual Model::StopLabelingJobOutcome StopLabelingJob(const Model::StopLabelingJobRequest& request) const;


        /**
         * <p>Stops a previously started monitoring schedule.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StopMonitoringSchedule">AWS
         * API Reference</a></p>
         */
        virtual Model::StopMonitoringScheduleOutcome StopMonitoringSchedule(const Model::StopMonitoringScheduleRequest& request) const;


        /**
         * <p>Terminates the ML compute instance. Before terminating the instance,
         * SageMaker disconnects the ML storage volume from it. SageMaker preserves the ML
         * storage volume. SageMaker stops charging you for the ML compute instance when
         * you call <code>StopNotebookInstance</code>.</p> <p>To access data on the ML
         * storage volume for a notebook instance that has been terminated, call the
         * <code>StartNotebookInstance</code> API. <code>StartNotebookInstance</code>
         * launches another ML compute instance, configures it, and attaches the preserved
         * ML storage volume so you can continue your work. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StopNotebookInstance">AWS
         * API Reference</a></p>
         */
        virtual Model::StopNotebookInstanceOutcome StopNotebookInstance(const Model::StopNotebookInstanceRequest& request) const;


        /**
         * <p>Stops a pipeline execution.</p> <p> <b>Callback Step</b> </p> <p>A pipeline
         * execution won't stop while a callback step is running. When you call
         * <code>StopPipelineExecution</code> on a pipeline execution with a running
         * callback step, SageMaker Pipelines sends an additional Amazon SQS message to the
         * specified SQS queue. The body of the SQS message contains a "Status" field which
         * is set to "Stopping".</p> <p>You should add logic to your Amazon SQS message
         * consumer to take any needed action (for example, resource cleanup) upon receipt
         * of the message followed by a call to
         * <code>SendPipelineExecutionStepSuccess</code> or
         * <code>SendPipelineExecutionStepFailure</code>.</p> <p>Only when SageMaker
         * Pipelines receives one of these calls will it stop the pipeline execution.</p>
         * <p> <b>Lambda Step</b> </p> <p>A pipeline execution can't be stopped while a
         * lambda step is running because the Lambda function invoked by the lambda step
         * can't be stopped. If you attempt to stop the execution while the Lambda function
         * is running, the pipeline waits for the Lambda function to finish or until the
         * timeout is hit, whichever occurs first, and then stops. If the Lambda function
         * finishes, the pipeline execution status is <code>Stopped</code>. If the timeout
         * is hit the pipeline execution status is <code>Failed</code>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StopPipelineExecution">AWS
         * API Reference</a></p>
         */
        virtual Model::StopPipelineExecutionOutcome StopPipelineExecution(const Model::StopPipelineExecutionRequest& request) const;


        /**
         * <p>Stops a processing job.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StopProcessingJob">AWS
         * API Reference</a></p>
         */
        virtual Model::StopProcessingJobOutcome StopProcessingJob(const Model::StopProcessingJobRequest& request) const;


        /**
         * <p>Stops a training job. To stop a job, SageMaker sends the algorithm the
         * <code>SIGTERM</code> signal, which delays job termination for 120 seconds.
         * Algorithms might use this 120-second window to save the model artifacts, so the
         * results of the training is not lost. </p> <p>When it receives a
         * <code>StopTrainingJob</code> request, SageMaker changes the status of the job to
         * <code>Stopping</code>. After SageMaker stops the job, it sets the status to
         * <code>Stopped</code>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StopTrainingJob">AWS
         * API Reference</a></p>
         */
        virtual Model::StopTrainingJobOutcome StopTrainingJob(const Model::StopTrainingJobRequest& request) const;


        /**
         * <p>Stops a batch transform job.</p> <p>When Amazon SageMaker receives a
         * <code>StopTransformJob</code> request, the status of the job changes to
         * <code>Stopping</code>. After Amazon SageMaker stops the job, the status is set
         * to <code>Stopped</code>. When you stop a batch transform job before it is
         * completed, Amazon SageMaker doesn't store the job's output in Amazon
         * S3.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/StopTransformJob">AWS
         * API Reference</a></p>
         */
        virtual Model::StopTransformJobOutcome StopTransformJob(const Model::StopTransformJobRequest& request) const;


        /**
         * <p>Updates an action.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateAction">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateActionOutcome UpdateAction(const Model::UpdateActionRequest& request) const;


        /**
         * <p>Updates the properties of an AppImageConfig.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateAppImageConfig">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateAppImageConfigOutcome UpdateAppImageConfig(const Model::UpdateAppImageConfigRequest& request) const;


        /**
         * <p>Updates an artifact.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateArtifact">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateArtifactOutcome UpdateArtifact(const Model::UpdateArtifactRequest& request) const;


        /**
         * <p>Updates the specified Git repository with the specified values.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateCodeRepository">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateCodeRepositoryOutcome UpdateCodeRepository(const Model::UpdateCodeRepositoryRequest& request) const;


        /**
         * <p>Updates a context.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateContext">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateContextOutcome UpdateContext(const Model::UpdateContextRequest& request) const;


        /**
         * <p>Updates a fleet of devices.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateDeviceFleet">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateDeviceFleetOutcome UpdateDeviceFleet(const Model::UpdateDeviceFleetRequest& request) const;


        /**
         * <p>Updates one or more devices in a fleet.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateDevices">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateDevicesOutcome UpdateDevices(const Model::UpdateDevicesRequest& request) const;


        /**
         * <p>Updates the default settings for new user profiles in the
         * domain.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateDomain">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateDomainOutcome UpdateDomain(const Model::UpdateDomainRequest& request) const;


        /**
         * <p>Deploys the new <code>EndpointConfig</code> specified in the request,
         * switches to using newly created endpoint, and then deletes resources provisioned
         * for the endpoint using the previous <code>EndpointConfig</code> (there is no
         * availability loss). </p> <p>When SageMaker receives the request, it sets the
         * endpoint status to <code>Updating</code>. After updating the endpoint, it sets
         * the status to <code>InService</code>. To check the status of an endpoint, use
         * the <a>DescribeEndpoint</a> API. </p>  <p>You must not delete an
         * <code>EndpointConfig</code> in use by an endpoint that is live or while the
         * <code>UpdateEndpoint</code> or <code>CreateEndpoint</code> operations are being
         * performed on the endpoint. To update an endpoint, you must create a new
         * <code>EndpointConfig</code>.</p> <p>If you delete the
         * <code>EndpointConfig</code> of an endpoint that is active or being created or
         * updated you may lose visibility into the instance type the endpoint is using.
         * The endpoint must be deleted in order to stop incurring charges.</p>
         * <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateEndpoint">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateEndpointOutcome UpdateEndpoint(const Model::UpdateEndpointRequest& request) const;


        /**
         * <p>Updates variant weight of one or more variants associated with an existing
         * endpoint, or capacity of one variant associated with an existing endpoint. When
         * it receives the request, SageMaker sets the endpoint status to
         * <code>Updating</code>. After updating the endpoint, it sets the status to
         * <code>InService</code>. To check the status of an endpoint, use the
         * <a>DescribeEndpoint</a> API. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateEndpointWeightsAndCapacities">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateEndpointWeightsAndCapacitiesOutcome UpdateEndpointWeightsAndCapacities(const Model::UpdateEndpointWeightsAndCapacitiesRequest& request) const;


        /**
         * <p>Adds, updates, or removes the description of an experiment. Updates the
         * display name of an experiment.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateExperiment">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateExperimentOutcome UpdateExperiment(const Model::UpdateExperimentRequest& request) const;


        /**
         * <p>Updates the feature group.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateFeatureGroup">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateFeatureGroupOutcome UpdateFeatureGroup(const Model::UpdateFeatureGroupRequest& request) const;


        /**
         * <p>Updates the description and parameters of the feature group.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateFeatureMetadata">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateFeatureMetadataOutcome UpdateFeatureMetadata(const Model::UpdateFeatureMetadataRequest& request) const;


        /**
         * <p>Updates the properties of a SageMaker image. To change the image's tags, use
         * the <a>AddTags</a> and <a>DeleteTags</a> APIs.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateImage">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateImageOutcome UpdateImage(const Model::UpdateImageRequest& request) const;


        /**
         * <p>Updates a versioned model.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateModelPackage">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateModelPackageOutcome UpdateModelPackage(const Model::UpdateModelPackageRequest& request) const;


        /**
         * <p>Updates a previously created schedule.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateMonitoringSchedule">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateMonitoringScheduleOutcome UpdateMonitoringSchedule(const Model::UpdateMonitoringScheduleRequest& request) const;


        /**
         * <p>Updates a notebook instance. NotebookInstance updates include upgrading or
         * downgrading the ML compute instance used for your notebook instance to
         * accommodate changes in your workload requirements.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateNotebookInstance">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateNotebookInstanceOutcome UpdateNotebookInstance(const Model::UpdateNotebookInstanceRequest& request) const;


        /**
         * <p>Updates a notebook instance lifecycle configuration created with the
         * <a>CreateNotebookInstanceLifecycleConfig</a> API.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateNotebookInstanceLifecycleConfig">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateNotebookInstanceLifecycleConfigOutcome UpdateNotebookInstanceLifecycleConfig(const Model::UpdateNotebookInstanceLifecycleConfigRequest& request) const;


        /**
         * <p>Updates a pipeline.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdatePipeline">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdatePipelineOutcome UpdatePipeline(const Model::UpdatePipelineRequest& request) const;


        /**
         * <p>Updates a pipeline execution.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdatePipelineExecution">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdatePipelineExecutionOutcome UpdatePipelineExecution(const Model::UpdatePipelineExecutionRequest& request) const;


        /**
         * <p>Updates a machine learning (ML) project that is created from a template that
         * sets up an ML pipeline from training to deploying an approved model.</p> 
         * <p>You must not update a project that is in use. If you update the
         * <code>ServiceCatalogProvisioningUpdateDetails</code> of a project that is active
         * or being created, or updated, you may lose resources already created by the
         * project.</p> <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateProject">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateProjectOutcome UpdateProject(const Model::UpdateProjectRequest& request) const;


        /**
         * <p>Update a model training job to request a new Debugger profiling configuration
         * or to change warm pool retention length.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateTrainingJob">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateTrainingJobOutcome UpdateTrainingJob(const Model::UpdateTrainingJobRequest& request) const;


        /**
         * <p>Updates the display name of a trial.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateTrial">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateTrialOutcome UpdateTrial(const Model::UpdateTrialRequest& request) const;


        /**
         * <p>Updates one or more properties of a trial component.</p><p><h3>See Also:</h3>
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateTrialComponent">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateTrialComponentOutcome UpdateTrialComponent(const Model::UpdateTrialComponentRequest& request) const;


        /**
         * <p>Updates a user profile.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateUserProfile">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateUserProfileOutcome UpdateUserProfile(const Model::UpdateUserProfileRequest& request) const;


        /**
         * <p>Use this operation to update your workforce. You can use this operation to
         * require that workers use specific IP addresses to work on tasks and to update
         * your OpenID Connect (OIDC) Identity Provider (IdP) workforce configuration.</p>
         * <p>The worker portal is now supported in VPC and public internet.</p> <p> Use
         * <code>SourceIpConfig</code> to restrict worker access to tasks to a specific
         * range of IP addresses. You specify allowed IP addresses by creating a list of up
         * to ten <a
         * href="https://docs.aws.amazon.com/vpc/latest/userguide/VPC_Subnets.html">CIDRs</a>.
         * By default, a workforce isn't restricted to specific IP addresses. If you
         * specify a range of IP addresses, workers who attempt to access tasks using any
         * IP address outside the specified range are denied and get a <code>Not
         * Found</code> error message on the worker portal.</p> <p>To restrict access to
         * all the workers in public internet, add the <code>SourceIpConfig</code> CIDR
         * value as "0.0.0.0/0".</p>  <p>Amazon SageMaker does not support
         * Source Ip restriction for worker portals in VPC.</p>  <p>Use
         * <code>OidcConfig</code> to update the configuration of a workforce created using
         * your own OIDC IdP. </p>  <p>You can only update your OIDC IdP
         * configuration when there are no work teams associated with your workforce. You
         * can delete work teams using the operation.</p>  <p>After restricting
         * access to a range of IP addresses or updating your OIDC IdP configuration with
         * this operation, you can view details about your update workforce using the
         * operation.</p>  <p>This operation only applies to private
         * workforces.</p> <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateWorkforce">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateWorkforceOutcome UpdateWorkforce(const Model::UpdateWorkforceRequest& request) const;


        /**
         * <p>Updates an existing work team with new member definitions or
         * description.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/sagemaker-2017-07-24/UpdateWorkteam">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateWorkteamOutcome UpdateWorkteam(const Model::UpdateWorkteamRequest& request) const;



      void OverrideEndpoint(const Aws::String& endpoint);
      std::shared_ptr<SageMakerEndpointProviderBase>& accessEndpointProvider();
    private:
      void init(const SageMakerClientConfiguration& clientConfiguration);

      SageMakerClientConfiguration m_clientConfiguration;
      std::shared_ptr<Aws::Utils::Threading::Executor> m_executor;
      std::shared_ptr<SageMakerEndpointProviderBase> m_endpointProvider;
  };

} // namespace SageMaker
} // namespace Aws
