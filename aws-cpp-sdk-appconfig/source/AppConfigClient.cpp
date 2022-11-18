﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/core/utils/Outcome.h>
#include <aws/core/auth/AWSAuthSigner.h>
#include <aws/core/client/CoreErrors.h>
#include <aws/core/client/RetryStrategy.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpResponse.h>
#include <aws/core/http/HttpClientFactory.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/core/utils/threading/Executor.h>
#include <aws/core/utils/DNS.h>
#include <aws/core/utils/logging/LogMacros.h>
#include <aws/core/utils/logging/ErrorMacros.h>

#include <aws/appconfig/AppConfigClient.h>
#include <aws/appconfig/AppConfigErrorMarshaller.h>
#include <aws/appconfig/AppConfigEndpointProvider.h>
#include <aws/appconfig/model/CreateApplicationRequest.h>
#include <aws/appconfig/model/CreateConfigurationProfileRequest.h>
#include <aws/appconfig/model/CreateDeploymentStrategyRequest.h>
#include <aws/appconfig/model/CreateEnvironmentRequest.h>
#include <aws/appconfig/model/CreateExtensionRequest.h>
#include <aws/appconfig/model/CreateExtensionAssociationRequest.h>
#include <aws/appconfig/model/CreateHostedConfigurationVersionRequest.h>
#include <aws/appconfig/model/DeleteApplicationRequest.h>
#include <aws/appconfig/model/DeleteConfigurationProfileRequest.h>
#include <aws/appconfig/model/DeleteDeploymentStrategyRequest.h>
#include <aws/appconfig/model/DeleteEnvironmentRequest.h>
#include <aws/appconfig/model/DeleteExtensionRequest.h>
#include <aws/appconfig/model/DeleteExtensionAssociationRequest.h>
#include <aws/appconfig/model/DeleteHostedConfigurationVersionRequest.h>
#include <aws/appconfig/model/GetApplicationRequest.h>
#include <aws/appconfig/model/GetConfigurationProfileRequest.h>
#include <aws/appconfig/model/GetDeploymentRequest.h>
#include <aws/appconfig/model/GetDeploymentStrategyRequest.h>
#include <aws/appconfig/model/GetEnvironmentRequest.h>
#include <aws/appconfig/model/GetExtensionRequest.h>
#include <aws/appconfig/model/GetExtensionAssociationRequest.h>
#include <aws/appconfig/model/GetHostedConfigurationVersionRequest.h>
#include <aws/appconfig/model/ListApplicationsRequest.h>
#include <aws/appconfig/model/ListConfigurationProfilesRequest.h>
#include <aws/appconfig/model/ListDeploymentStrategiesRequest.h>
#include <aws/appconfig/model/ListDeploymentsRequest.h>
#include <aws/appconfig/model/ListEnvironmentsRequest.h>
#include <aws/appconfig/model/ListExtensionAssociationsRequest.h>
#include <aws/appconfig/model/ListExtensionsRequest.h>
#include <aws/appconfig/model/ListHostedConfigurationVersionsRequest.h>
#include <aws/appconfig/model/ListTagsForResourceRequest.h>
#include <aws/appconfig/model/StartDeploymentRequest.h>
#include <aws/appconfig/model/StopDeploymentRequest.h>
#include <aws/appconfig/model/TagResourceRequest.h>
#include <aws/appconfig/model/UntagResourceRequest.h>
#include <aws/appconfig/model/UpdateApplicationRequest.h>
#include <aws/appconfig/model/UpdateConfigurationProfileRequest.h>
#include <aws/appconfig/model/UpdateDeploymentStrategyRequest.h>
#include <aws/appconfig/model/UpdateEnvironmentRequest.h>
#include <aws/appconfig/model/UpdateExtensionRequest.h>
#include <aws/appconfig/model/UpdateExtensionAssociationRequest.h>
#include <aws/appconfig/model/ValidateConfigurationRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::AppConfig;
using namespace Aws::AppConfig::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* AppConfigClient::SERVICE_NAME = "appconfig";
const char* AppConfigClient::ALLOCATION_TAG = "AppConfigClient";

AppConfigClient::AppConfigClient(const AppConfig::AppConfigClientConfiguration& clientConfiguration,
                                 std::shared_ptr<AppConfigEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AppConfigErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

AppConfigClient::AppConfigClient(const AWSCredentials& credentials,
                                 std::shared_ptr<AppConfigEndpointProviderBase> endpointProvider,
                                 const AppConfig::AppConfigClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AppConfigErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

AppConfigClient::AppConfigClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                 std::shared_ptr<AppConfigEndpointProviderBase> endpointProvider,
                                 const AppConfig::AppConfigClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  credentialsProvider,
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AppConfigErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  AppConfigClient::AppConfigClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AppConfigErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<AppConfigEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

AppConfigClient::AppConfigClient(const AWSCredentials& credentials,
                                 const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AppConfigErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<AppConfigEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

AppConfigClient::AppConfigClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                 const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  credentialsProvider,
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AppConfigErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<AppConfigEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
AppConfigClient::~AppConfigClient()
{
}

std::shared_ptr<AppConfigEndpointProviderBase>& AppConfigClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void AppConfigClient::init(const AppConfig::AppConfigClientConfiguration& config)
{
  AWSClient::SetServiceClientName("AppConfig");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void AppConfigClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CreateApplicationOutcome AppConfigClient::CreateApplication(const CreateApplicationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications");
  return CreateApplicationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateApplicationOutcomeCallable AppConfigClient::CreateApplicationCallable(const CreateApplicationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateApplication, request, m_executor.get());
}

void AppConfigClient::CreateApplicationAsync(const CreateApplicationRequest& request, const CreateApplicationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateApplication, request, handler, context, m_executor.get());
}

CreateConfigurationProfileOutcome AppConfigClient::CreateConfigurationProfile(const CreateConfigurationProfileRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateConfigurationProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateConfigurationProfile", "Required field: ApplicationId, is not set");
    return CreateConfigurationProfileOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateConfigurationProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/configurationprofiles");
  return CreateConfigurationProfileOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateConfigurationProfileOutcomeCallable AppConfigClient::CreateConfigurationProfileCallable(const CreateConfigurationProfileRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateConfigurationProfile, request, m_executor.get());
}

void AppConfigClient::CreateConfigurationProfileAsync(const CreateConfigurationProfileRequest& request, const CreateConfigurationProfileResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateConfigurationProfile, request, handler, context, m_executor.get());
}

CreateDeploymentStrategyOutcome AppConfigClient::CreateDeploymentStrategy(const CreateDeploymentStrategyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDeploymentStrategy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDeploymentStrategy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deploymentstrategies");
  return CreateDeploymentStrategyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateDeploymentStrategyOutcomeCallable AppConfigClient::CreateDeploymentStrategyCallable(const CreateDeploymentStrategyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateDeploymentStrategy, request, m_executor.get());
}

void AppConfigClient::CreateDeploymentStrategyAsync(const CreateDeploymentStrategyRequest& request, const CreateDeploymentStrategyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateDeploymentStrategy, request, handler, context, m_executor.get());
}

CreateEnvironmentOutcome AppConfigClient::CreateEnvironment(const CreateEnvironmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateEnvironment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateEnvironment", "Required field: ApplicationId, is not set");
    return CreateEnvironmentOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateEnvironment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environments");
  return CreateEnvironmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateEnvironmentOutcomeCallable AppConfigClient::CreateEnvironmentCallable(const CreateEnvironmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateEnvironment, request, m_executor.get());
}

void AppConfigClient::CreateEnvironmentAsync(const CreateEnvironmentRequest& request, const CreateEnvironmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateEnvironment, request, handler, context, m_executor.get());
}

CreateExtensionOutcome AppConfigClient::CreateExtension(const CreateExtensionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateExtension, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateExtension, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/extensions");
  return CreateExtensionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateExtensionOutcomeCallable AppConfigClient::CreateExtensionCallable(const CreateExtensionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateExtension, request, m_executor.get());
}

void AppConfigClient::CreateExtensionAsync(const CreateExtensionRequest& request, const CreateExtensionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateExtension, request, handler, context, m_executor.get());
}

CreateExtensionAssociationOutcome AppConfigClient::CreateExtensionAssociation(const CreateExtensionAssociationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateExtensionAssociation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateExtensionAssociation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/extensionassociations");
  return CreateExtensionAssociationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateExtensionAssociationOutcomeCallable AppConfigClient::CreateExtensionAssociationCallable(const CreateExtensionAssociationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateExtensionAssociation, request, m_executor.get());
}

void AppConfigClient::CreateExtensionAssociationAsync(const CreateExtensionAssociationRequest& request, const CreateExtensionAssociationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateExtensionAssociation, request, handler, context, m_executor.get());
}

CreateHostedConfigurationVersionOutcome AppConfigClient::CreateHostedConfigurationVersion(const CreateHostedConfigurationVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateHostedConfigurationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateHostedConfigurationVersion", "Required field: ApplicationId, is not set");
    return CreateHostedConfigurationVersionOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  if (!request.ConfigurationProfileIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateHostedConfigurationVersion", "Required field: ConfigurationProfileId, is not set");
    return CreateHostedConfigurationVersionOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConfigurationProfileId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateHostedConfigurationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/configurationprofiles/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConfigurationProfileId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/hostedconfigurationversions");
  return CreateHostedConfigurationVersionOutcome(MakeRequestWithUnparsedResponse(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateHostedConfigurationVersionOutcomeCallable AppConfigClient::CreateHostedConfigurationVersionCallable(const CreateHostedConfigurationVersionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateHostedConfigurationVersion, request, m_executor.get());
}

void AppConfigClient::CreateHostedConfigurationVersionAsync(const CreateHostedConfigurationVersionRequest& request, const CreateHostedConfigurationVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateHostedConfigurationVersion, request, handler, context, m_executor.get());
}

DeleteApplicationOutcome AppConfigClient::DeleteApplication(const DeleteApplicationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteApplication", "Required field: ApplicationId, is not set");
    return DeleteApplicationOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  return DeleteApplicationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteApplicationOutcomeCallable AppConfigClient::DeleteApplicationCallable(const DeleteApplicationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteApplication, request, m_executor.get());
}

void AppConfigClient::DeleteApplicationAsync(const DeleteApplicationRequest& request, const DeleteApplicationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteApplication, request, handler, context, m_executor.get());
}

DeleteConfigurationProfileOutcome AppConfigClient::DeleteConfigurationProfile(const DeleteConfigurationProfileRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteConfigurationProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteConfigurationProfile", "Required field: ApplicationId, is not set");
    return DeleteConfigurationProfileOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  if (!request.ConfigurationProfileIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteConfigurationProfile", "Required field: ConfigurationProfileId, is not set");
    return DeleteConfigurationProfileOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConfigurationProfileId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteConfigurationProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/configurationprofiles/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConfigurationProfileId());
  return DeleteConfigurationProfileOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteConfigurationProfileOutcomeCallable AppConfigClient::DeleteConfigurationProfileCallable(const DeleteConfigurationProfileRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteConfigurationProfile, request, m_executor.get());
}

void AppConfigClient::DeleteConfigurationProfileAsync(const DeleteConfigurationProfileRequest& request, const DeleteConfigurationProfileResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteConfigurationProfile, request, handler, context, m_executor.get());
}

DeleteDeploymentStrategyOutcome AppConfigClient::DeleteDeploymentStrategy(const DeleteDeploymentStrategyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDeploymentStrategy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DeploymentStrategyIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDeploymentStrategy", "Required field: DeploymentStrategyId, is not set");
    return DeleteDeploymentStrategyOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DeploymentStrategyId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDeploymentStrategy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deployementstrategies/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDeploymentStrategyId());
  return DeleteDeploymentStrategyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteDeploymentStrategyOutcomeCallable AppConfigClient::DeleteDeploymentStrategyCallable(const DeleteDeploymentStrategyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteDeploymentStrategy, request, m_executor.get());
}

void AppConfigClient::DeleteDeploymentStrategyAsync(const DeleteDeploymentStrategyRequest& request, const DeleteDeploymentStrategyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteDeploymentStrategy, request, handler, context, m_executor.get());
}

DeleteEnvironmentOutcome AppConfigClient::DeleteEnvironment(const DeleteEnvironmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteEnvironment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteEnvironment", "Required field: ApplicationId, is not set");
    return DeleteEnvironmentOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  if (!request.EnvironmentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteEnvironment", "Required field: EnvironmentId, is not set");
    return DeleteEnvironmentOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteEnvironment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentId());
  return DeleteEnvironmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteEnvironmentOutcomeCallable AppConfigClient::DeleteEnvironmentCallable(const DeleteEnvironmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteEnvironment, request, m_executor.get());
}

void AppConfigClient::DeleteEnvironmentAsync(const DeleteEnvironmentRequest& request, const DeleteEnvironmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteEnvironment, request, handler, context, m_executor.get());
}

DeleteExtensionOutcome AppConfigClient::DeleteExtension(const DeleteExtensionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteExtension, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ExtensionIdentifierHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteExtension", "Required field: ExtensionIdentifier, is not set");
    return DeleteExtensionOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ExtensionIdentifier]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteExtension, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/extensions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetExtensionIdentifier());
  return DeleteExtensionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteExtensionOutcomeCallable AppConfigClient::DeleteExtensionCallable(const DeleteExtensionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteExtension, request, m_executor.get());
}

void AppConfigClient::DeleteExtensionAsync(const DeleteExtensionRequest& request, const DeleteExtensionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteExtension, request, handler, context, m_executor.get());
}

DeleteExtensionAssociationOutcome AppConfigClient::DeleteExtensionAssociation(const DeleteExtensionAssociationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteExtensionAssociation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ExtensionAssociationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteExtensionAssociation", "Required field: ExtensionAssociationId, is not set");
    return DeleteExtensionAssociationOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ExtensionAssociationId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteExtensionAssociation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/extensionassociations/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetExtensionAssociationId());
  return DeleteExtensionAssociationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteExtensionAssociationOutcomeCallable AppConfigClient::DeleteExtensionAssociationCallable(const DeleteExtensionAssociationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteExtensionAssociation, request, m_executor.get());
}

void AppConfigClient::DeleteExtensionAssociationAsync(const DeleteExtensionAssociationRequest& request, const DeleteExtensionAssociationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteExtensionAssociation, request, handler, context, m_executor.get());
}

DeleteHostedConfigurationVersionOutcome AppConfigClient::DeleteHostedConfigurationVersion(const DeleteHostedConfigurationVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteHostedConfigurationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteHostedConfigurationVersion", "Required field: ApplicationId, is not set");
    return DeleteHostedConfigurationVersionOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  if (!request.ConfigurationProfileIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteHostedConfigurationVersion", "Required field: ConfigurationProfileId, is not set");
    return DeleteHostedConfigurationVersionOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConfigurationProfileId]", false));
  }
  if (!request.VersionNumberHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteHostedConfigurationVersion", "Required field: VersionNumber, is not set");
    return DeleteHostedConfigurationVersionOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VersionNumber]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteHostedConfigurationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/configurationprofiles/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConfigurationProfileId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/hostedconfigurationversions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVersionNumber());
  return DeleteHostedConfigurationVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteHostedConfigurationVersionOutcomeCallable AppConfigClient::DeleteHostedConfigurationVersionCallable(const DeleteHostedConfigurationVersionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteHostedConfigurationVersion, request, m_executor.get());
}

void AppConfigClient::DeleteHostedConfigurationVersionAsync(const DeleteHostedConfigurationVersionRequest& request, const DeleteHostedConfigurationVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteHostedConfigurationVersion, request, handler, context, m_executor.get());
}

GetApplicationOutcome AppConfigClient::GetApplication(const GetApplicationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetApplication", "Required field: ApplicationId, is not set");
    return GetApplicationOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  return GetApplicationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetApplicationOutcomeCallable AppConfigClient::GetApplicationCallable(const GetApplicationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetApplication, request, m_executor.get());
}

void AppConfigClient::GetApplicationAsync(const GetApplicationRequest& request, const GetApplicationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetApplication, request, handler, context, m_executor.get());
}

GetConfigurationProfileOutcome AppConfigClient::GetConfigurationProfile(const GetConfigurationProfileRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetConfigurationProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetConfigurationProfile", "Required field: ApplicationId, is not set");
    return GetConfigurationProfileOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  if (!request.ConfigurationProfileIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetConfigurationProfile", "Required field: ConfigurationProfileId, is not set");
    return GetConfigurationProfileOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConfigurationProfileId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetConfigurationProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/configurationprofiles/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConfigurationProfileId());
  return GetConfigurationProfileOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetConfigurationProfileOutcomeCallable AppConfigClient::GetConfigurationProfileCallable(const GetConfigurationProfileRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetConfigurationProfile, request, m_executor.get());
}

void AppConfigClient::GetConfigurationProfileAsync(const GetConfigurationProfileRequest& request, const GetConfigurationProfileResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetConfigurationProfile, request, handler, context, m_executor.get());
}

GetDeploymentOutcome AppConfigClient::GetDeployment(const GetDeploymentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDeployment", "Required field: ApplicationId, is not set");
    return GetDeploymentOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  if (!request.EnvironmentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDeployment", "Required field: EnvironmentId, is not set");
    return GetDeploymentOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentId]", false));
  }
  if (!request.DeploymentNumberHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDeployment", "Required field: DeploymentNumber, is not set");
    return GetDeploymentOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DeploymentNumber]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deployments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDeploymentNumber());
  return GetDeploymentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetDeploymentOutcomeCallable AppConfigClient::GetDeploymentCallable(const GetDeploymentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetDeployment, request, m_executor.get());
}

void AppConfigClient::GetDeploymentAsync(const GetDeploymentRequest& request, const GetDeploymentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetDeployment, request, handler, context, m_executor.get());
}

GetDeploymentStrategyOutcome AppConfigClient::GetDeploymentStrategy(const GetDeploymentStrategyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDeploymentStrategy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DeploymentStrategyIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDeploymentStrategy", "Required field: DeploymentStrategyId, is not set");
    return GetDeploymentStrategyOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DeploymentStrategyId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDeploymentStrategy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deploymentstrategies/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDeploymentStrategyId());
  return GetDeploymentStrategyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetDeploymentStrategyOutcomeCallable AppConfigClient::GetDeploymentStrategyCallable(const GetDeploymentStrategyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetDeploymentStrategy, request, m_executor.get());
}

void AppConfigClient::GetDeploymentStrategyAsync(const GetDeploymentStrategyRequest& request, const GetDeploymentStrategyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetDeploymentStrategy, request, handler, context, m_executor.get());
}

GetEnvironmentOutcome AppConfigClient::GetEnvironment(const GetEnvironmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetEnvironment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetEnvironment", "Required field: ApplicationId, is not set");
    return GetEnvironmentOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  if (!request.EnvironmentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetEnvironment", "Required field: EnvironmentId, is not set");
    return GetEnvironmentOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetEnvironment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentId());
  return GetEnvironmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetEnvironmentOutcomeCallable AppConfigClient::GetEnvironmentCallable(const GetEnvironmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetEnvironment, request, m_executor.get());
}

void AppConfigClient::GetEnvironmentAsync(const GetEnvironmentRequest& request, const GetEnvironmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetEnvironment, request, handler, context, m_executor.get());
}

GetExtensionOutcome AppConfigClient::GetExtension(const GetExtensionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetExtension, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ExtensionIdentifierHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExtension", "Required field: ExtensionIdentifier, is not set");
    return GetExtensionOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ExtensionIdentifier]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetExtension, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/extensions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetExtensionIdentifier());
  return GetExtensionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetExtensionOutcomeCallable AppConfigClient::GetExtensionCallable(const GetExtensionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetExtension, request, m_executor.get());
}

void AppConfigClient::GetExtensionAsync(const GetExtensionRequest& request, const GetExtensionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetExtension, request, handler, context, m_executor.get());
}

GetExtensionAssociationOutcome AppConfigClient::GetExtensionAssociation(const GetExtensionAssociationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetExtensionAssociation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ExtensionAssociationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExtensionAssociation", "Required field: ExtensionAssociationId, is not set");
    return GetExtensionAssociationOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ExtensionAssociationId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetExtensionAssociation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/extensionassociations/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetExtensionAssociationId());
  return GetExtensionAssociationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetExtensionAssociationOutcomeCallable AppConfigClient::GetExtensionAssociationCallable(const GetExtensionAssociationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetExtensionAssociation, request, m_executor.get());
}

void AppConfigClient::GetExtensionAssociationAsync(const GetExtensionAssociationRequest& request, const GetExtensionAssociationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetExtensionAssociation, request, handler, context, m_executor.get());
}

GetHostedConfigurationVersionOutcome AppConfigClient::GetHostedConfigurationVersion(const GetHostedConfigurationVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetHostedConfigurationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetHostedConfigurationVersion", "Required field: ApplicationId, is not set");
    return GetHostedConfigurationVersionOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  if (!request.ConfigurationProfileIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetHostedConfigurationVersion", "Required field: ConfigurationProfileId, is not set");
    return GetHostedConfigurationVersionOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConfigurationProfileId]", false));
  }
  if (!request.VersionNumberHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetHostedConfigurationVersion", "Required field: VersionNumber, is not set");
    return GetHostedConfigurationVersionOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VersionNumber]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetHostedConfigurationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/configurationprofiles/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConfigurationProfileId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/hostedconfigurationversions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVersionNumber());
  return GetHostedConfigurationVersionOutcome(MakeRequestWithUnparsedResponse(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET));
}

GetHostedConfigurationVersionOutcomeCallable AppConfigClient::GetHostedConfigurationVersionCallable(const GetHostedConfigurationVersionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetHostedConfigurationVersion, request, m_executor.get());
}

void AppConfigClient::GetHostedConfigurationVersionAsync(const GetHostedConfigurationVersionRequest& request, const GetHostedConfigurationVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetHostedConfigurationVersion, request, handler, context, m_executor.get());
}

ListApplicationsOutcome AppConfigClient::ListApplications(const ListApplicationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListApplications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListApplications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications");
  return ListApplicationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListApplicationsOutcomeCallable AppConfigClient::ListApplicationsCallable(const ListApplicationsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListApplications, request, m_executor.get());
}

void AppConfigClient::ListApplicationsAsync(const ListApplicationsRequest& request, const ListApplicationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListApplications, request, handler, context, m_executor.get());
}

ListConfigurationProfilesOutcome AppConfigClient::ListConfigurationProfiles(const ListConfigurationProfilesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListConfigurationProfiles, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListConfigurationProfiles", "Required field: ApplicationId, is not set");
    return ListConfigurationProfilesOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListConfigurationProfiles, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/configurationprofiles");
  return ListConfigurationProfilesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListConfigurationProfilesOutcomeCallable AppConfigClient::ListConfigurationProfilesCallable(const ListConfigurationProfilesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListConfigurationProfiles, request, m_executor.get());
}

void AppConfigClient::ListConfigurationProfilesAsync(const ListConfigurationProfilesRequest& request, const ListConfigurationProfilesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListConfigurationProfiles, request, handler, context, m_executor.get());
}

ListDeploymentStrategiesOutcome AppConfigClient::ListDeploymentStrategies(const ListDeploymentStrategiesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListDeploymentStrategies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListDeploymentStrategies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deploymentstrategies");
  return ListDeploymentStrategiesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListDeploymentStrategiesOutcomeCallable AppConfigClient::ListDeploymentStrategiesCallable(const ListDeploymentStrategiesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListDeploymentStrategies, request, m_executor.get());
}

void AppConfigClient::ListDeploymentStrategiesAsync(const ListDeploymentStrategiesRequest& request, const ListDeploymentStrategiesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListDeploymentStrategies, request, handler, context, m_executor.get());
}

ListDeploymentsOutcome AppConfigClient::ListDeployments(const ListDeploymentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListDeployments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListDeployments", "Required field: ApplicationId, is not set");
    return ListDeploymentsOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  if (!request.EnvironmentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListDeployments", "Required field: EnvironmentId, is not set");
    return ListDeploymentsOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListDeployments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deployments");
  return ListDeploymentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListDeploymentsOutcomeCallable AppConfigClient::ListDeploymentsCallable(const ListDeploymentsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListDeployments, request, m_executor.get());
}

void AppConfigClient::ListDeploymentsAsync(const ListDeploymentsRequest& request, const ListDeploymentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListDeployments, request, handler, context, m_executor.get());
}

ListEnvironmentsOutcome AppConfigClient::ListEnvironments(const ListEnvironmentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListEnvironments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListEnvironments", "Required field: ApplicationId, is not set");
    return ListEnvironmentsOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListEnvironments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environments");
  return ListEnvironmentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListEnvironmentsOutcomeCallable AppConfigClient::ListEnvironmentsCallable(const ListEnvironmentsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListEnvironments, request, m_executor.get());
}

void AppConfigClient::ListEnvironmentsAsync(const ListEnvironmentsRequest& request, const ListEnvironmentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListEnvironments, request, handler, context, m_executor.get());
}

ListExtensionAssociationsOutcome AppConfigClient::ListExtensionAssociations(const ListExtensionAssociationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListExtensionAssociations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListExtensionAssociations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/extensionassociations");
  return ListExtensionAssociationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListExtensionAssociationsOutcomeCallable AppConfigClient::ListExtensionAssociationsCallable(const ListExtensionAssociationsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListExtensionAssociations, request, m_executor.get());
}

void AppConfigClient::ListExtensionAssociationsAsync(const ListExtensionAssociationsRequest& request, const ListExtensionAssociationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListExtensionAssociations, request, handler, context, m_executor.get());
}

ListExtensionsOutcome AppConfigClient::ListExtensions(const ListExtensionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListExtensions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListExtensions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/extensions");
  return ListExtensionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListExtensionsOutcomeCallable AppConfigClient::ListExtensionsCallable(const ListExtensionsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListExtensions, request, m_executor.get());
}

void AppConfigClient::ListExtensionsAsync(const ListExtensionsRequest& request, const ListExtensionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListExtensions, request, handler, context, m_executor.get());
}

ListHostedConfigurationVersionsOutcome AppConfigClient::ListHostedConfigurationVersions(const ListHostedConfigurationVersionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListHostedConfigurationVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListHostedConfigurationVersions", "Required field: ApplicationId, is not set");
    return ListHostedConfigurationVersionsOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  if (!request.ConfigurationProfileIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListHostedConfigurationVersions", "Required field: ConfigurationProfileId, is not set");
    return ListHostedConfigurationVersionsOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConfigurationProfileId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListHostedConfigurationVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/configurationprofiles/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConfigurationProfileId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/hostedconfigurationversions");
  return ListHostedConfigurationVersionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListHostedConfigurationVersionsOutcomeCallable AppConfigClient::ListHostedConfigurationVersionsCallable(const ListHostedConfigurationVersionsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListHostedConfigurationVersions, request, m_executor.get());
}

void AppConfigClient::ListHostedConfigurationVersionsAsync(const ListHostedConfigurationVersionsRequest& request, const ListHostedConfigurationVersionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListHostedConfigurationVersions, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome AppConfigClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable AppConfigClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListTagsForResource, request, m_executor.get());
}

void AppConfigClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListTagsForResource, request, handler, context, m_executor.get());
}

StartDeploymentOutcome AppConfigClient::StartDeployment(const StartDeploymentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartDeployment", "Required field: ApplicationId, is not set");
    return StartDeploymentOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  if (!request.EnvironmentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartDeployment", "Required field: EnvironmentId, is not set");
    return StartDeploymentOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deployments");
  return StartDeploymentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartDeploymentOutcomeCallable AppConfigClient::StartDeploymentCallable(const StartDeploymentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(StartDeployment, request, m_executor.get());
}

void AppConfigClient::StartDeploymentAsync(const StartDeploymentRequest& request, const StartDeploymentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(StartDeployment, request, handler, context, m_executor.get());
}

StopDeploymentOutcome AppConfigClient::StopDeployment(const StopDeploymentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StopDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StopDeployment", "Required field: ApplicationId, is not set");
    return StopDeploymentOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  if (!request.EnvironmentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StopDeployment", "Required field: EnvironmentId, is not set");
    return StopDeploymentOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentId]", false));
  }
  if (!request.DeploymentNumberHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StopDeployment", "Required field: DeploymentNumber, is not set");
    return StopDeploymentOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DeploymentNumber]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StopDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deployments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDeploymentNumber());
  return StopDeploymentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

StopDeploymentOutcomeCallable AppConfigClient::StopDeploymentCallable(const StopDeploymentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(StopDeployment, request, m_executor.get());
}

void AppConfigClient::StopDeploymentAsync(const StopDeploymentRequest& request, const StopDeploymentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(StopDeployment, request, handler, context, m_executor.get());
}

TagResourceOutcome AppConfigClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable AppConfigClient::TagResourceCallable(const TagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(TagResource, request, m_executor.get());
}

void AppConfigClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(TagResource, request, handler, context, m_executor.get());
}

UntagResourceOutcome AppConfigClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable AppConfigClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UntagResource, request, m_executor.get());
}

void AppConfigClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UntagResource, request, handler, context, m_executor.get());
}

UpdateApplicationOutcome AppConfigClient::UpdateApplication(const UpdateApplicationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateApplication", "Required field: ApplicationId, is not set");
    return UpdateApplicationOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  return UpdateApplicationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateApplicationOutcomeCallable AppConfigClient::UpdateApplicationCallable(const UpdateApplicationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateApplication, request, m_executor.get());
}

void AppConfigClient::UpdateApplicationAsync(const UpdateApplicationRequest& request, const UpdateApplicationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateApplication, request, handler, context, m_executor.get());
}

UpdateConfigurationProfileOutcome AppConfigClient::UpdateConfigurationProfile(const UpdateConfigurationProfileRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateConfigurationProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateConfigurationProfile", "Required field: ApplicationId, is not set");
    return UpdateConfigurationProfileOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  if (!request.ConfigurationProfileIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateConfigurationProfile", "Required field: ConfigurationProfileId, is not set");
    return UpdateConfigurationProfileOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConfigurationProfileId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateConfigurationProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/configurationprofiles/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConfigurationProfileId());
  return UpdateConfigurationProfileOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateConfigurationProfileOutcomeCallable AppConfigClient::UpdateConfigurationProfileCallable(const UpdateConfigurationProfileRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateConfigurationProfile, request, m_executor.get());
}

void AppConfigClient::UpdateConfigurationProfileAsync(const UpdateConfigurationProfileRequest& request, const UpdateConfigurationProfileResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateConfigurationProfile, request, handler, context, m_executor.get());
}

UpdateDeploymentStrategyOutcome AppConfigClient::UpdateDeploymentStrategy(const UpdateDeploymentStrategyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateDeploymentStrategy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DeploymentStrategyIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDeploymentStrategy", "Required field: DeploymentStrategyId, is not set");
    return UpdateDeploymentStrategyOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DeploymentStrategyId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateDeploymentStrategy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deploymentstrategies/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDeploymentStrategyId());
  return UpdateDeploymentStrategyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateDeploymentStrategyOutcomeCallable AppConfigClient::UpdateDeploymentStrategyCallable(const UpdateDeploymentStrategyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateDeploymentStrategy, request, m_executor.get());
}

void AppConfigClient::UpdateDeploymentStrategyAsync(const UpdateDeploymentStrategyRequest& request, const UpdateDeploymentStrategyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateDeploymentStrategy, request, handler, context, m_executor.get());
}

UpdateEnvironmentOutcome AppConfigClient::UpdateEnvironment(const UpdateEnvironmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateEnvironment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateEnvironment", "Required field: ApplicationId, is not set");
    return UpdateEnvironmentOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  if (!request.EnvironmentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateEnvironment", "Required field: EnvironmentId, is not set");
    return UpdateEnvironmentOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateEnvironment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentId());
  return UpdateEnvironmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateEnvironmentOutcomeCallable AppConfigClient::UpdateEnvironmentCallable(const UpdateEnvironmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateEnvironment, request, m_executor.get());
}

void AppConfigClient::UpdateEnvironmentAsync(const UpdateEnvironmentRequest& request, const UpdateEnvironmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateEnvironment, request, handler, context, m_executor.get());
}

UpdateExtensionOutcome AppConfigClient::UpdateExtension(const UpdateExtensionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateExtension, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ExtensionIdentifierHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateExtension", "Required field: ExtensionIdentifier, is not set");
    return UpdateExtensionOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ExtensionIdentifier]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateExtension, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/extensions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetExtensionIdentifier());
  return UpdateExtensionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateExtensionOutcomeCallable AppConfigClient::UpdateExtensionCallable(const UpdateExtensionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateExtension, request, m_executor.get());
}

void AppConfigClient::UpdateExtensionAsync(const UpdateExtensionRequest& request, const UpdateExtensionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateExtension, request, handler, context, m_executor.get());
}

UpdateExtensionAssociationOutcome AppConfigClient::UpdateExtensionAssociation(const UpdateExtensionAssociationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateExtensionAssociation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ExtensionAssociationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateExtensionAssociation", "Required field: ExtensionAssociationId, is not set");
    return UpdateExtensionAssociationOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ExtensionAssociationId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateExtensionAssociation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/extensionassociations/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetExtensionAssociationId());
  return UpdateExtensionAssociationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateExtensionAssociationOutcomeCallable AppConfigClient::UpdateExtensionAssociationCallable(const UpdateExtensionAssociationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateExtensionAssociation, request, m_executor.get());
}

void AppConfigClient::UpdateExtensionAssociationAsync(const UpdateExtensionAssociationRequest& request, const UpdateExtensionAssociationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateExtensionAssociation, request, handler, context, m_executor.get());
}

ValidateConfigurationOutcome AppConfigClient::ValidateConfiguration(const ValidateConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ValidateConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ValidateConfiguration", "Required field: ApplicationId, is not set");
    return ValidateConfigurationOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationId]", false));
  }
  if (!request.ConfigurationProfileIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ValidateConfiguration", "Required field: ConfigurationProfileId, is not set");
    return ValidateConfigurationOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConfigurationProfileId]", false));
  }
  if (!request.ConfigurationVersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ValidateConfiguration", "Required field: ConfigurationVersion, is not set");
    return ValidateConfigurationOutcome(Aws::Client::AWSError<AppConfigErrors>(AppConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConfigurationVersion]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ValidateConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/applications/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/configurationprofiles/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConfigurationProfileId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/validators");
  return ValidateConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ValidateConfigurationOutcomeCallable AppConfigClient::ValidateConfigurationCallable(const ValidateConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ValidateConfiguration, request, m_executor.get());
}

void AppConfigClient::ValidateConfigurationAsync(const ValidateConfigurationRequest& request, const ValidateConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ValidateConfiguration, request, handler, context, m_executor.get());
}

