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

#include <aws/apigateway/APIGatewayClient.h>
#include <aws/apigateway/APIGatewayErrorMarshaller.h>
#include <aws/apigateway/APIGatewayEndpointProvider.h>
#include <aws/apigateway/model/CreateApiKeyRequest.h>
#include <aws/apigateway/model/CreateAuthorizerRequest.h>
#include <aws/apigateway/model/CreateBasePathMappingRequest.h>
#include <aws/apigateway/model/CreateDeploymentRequest.h>
#include <aws/apigateway/model/CreateDocumentationPartRequest.h>
#include <aws/apigateway/model/CreateDocumentationVersionRequest.h>
#include <aws/apigateway/model/CreateDomainNameRequest.h>
#include <aws/apigateway/model/CreateModelRequest.h>
#include <aws/apigateway/model/CreateRequestValidatorRequest.h>
#include <aws/apigateway/model/CreateResourceRequest.h>
#include <aws/apigateway/model/CreateRestApiRequest.h>
#include <aws/apigateway/model/CreateStageRequest.h>
#include <aws/apigateway/model/CreateUsagePlanRequest.h>
#include <aws/apigateway/model/CreateUsagePlanKeyRequest.h>
#include <aws/apigateway/model/CreateVpcLinkRequest.h>
#include <aws/apigateway/model/DeleteApiKeyRequest.h>
#include <aws/apigateway/model/DeleteAuthorizerRequest.h>
#include <aws/apigateway/model/DeleteBasePathMappingRequest.h>
#include <aws/apigateway/model/DeleteClientCertificateRequest.h>
#include <aws/apigateway/model/DeleteDeploymentRequest.h>
#include <aws/apigateway/model/DeleteDocumentationPartRequest.h>
#include <aws/apigateway/model/DeleteDocumentationVersionRequest.h>
#include <aws/apigateway/model/DeleteDomainNameRequest.h>
#include <aws/apigateway/model/DeleteGatewayResponseRequest.h>
#include <aws/apigateway/model/DeleteIntegrationRequest.h>
#include <aws/apigateway/model/DeleteIntegrationResponseRequest.h>
#include <aws/apigateway/model/DeleteMethodRequest.h>
#include <aws/apigateway/model/DeleteMethodResponseRequest.h>
#include <aws/apigateway/model/DeleteModelRequest.h>
#include <aws/apigateway/model/DeleteRequestValidatorRequest.h>
#include <aws/apigateway/model/DeleteResourceRequest.h>
#include <aws/apigateway/model/DeleteRestApiRequest.h>
#include <aws/apigateway/model/DeleteStageRequest.h>
#include <aws/apigateway/model/DeleteUsagePlanRequest.h>
#include <aws/apigateway/model/DeleteUsagePlanKeyRequest.h>
#include <aws/apigateway/model/DeleteVpcLinkRequest.h>
#include <aws/apigateway/model/FlushStageAuthorizersCacheRequest.h>
#include <aws/apigateway/model/FlushStageCacheRequest.h>
#include <aws/apigateway/model/GenerateClientCertificateRequest.h>
#include <aws/apigateway/model/GetAccountRequest.h>
#include <aws/apigateway/model/GetApiKeyRequest.h>
#include <aws/apigateway/model/GetApiKeysRequest.h>
#include <aws/apigateway/model/GetAuthorizerRequest.h>
#include <aws/apigateway/model/GetAuthorizersRequest.h>
#include <aws/apigateway/model/GetBasePathMappingRequest.h>
#include <aws/apigateway/model/GetBasePathMappingsRequest.h>
#include <aws/apigateway/model/GetClientCertificateRequest.h>
#include <aws/apigateway/model/GetClientCertificatesRequest.h>
#include <aws/apigateway/model/GetDeploymentRequest.h>
#include <aws/apigateway/model/GetDeploymentsRequest.h>
#include <aws/apigateway/model/GetDocumentationPartRequest.h>
#include <aws/apigateway/model/GetDocumentationPartsRequest.h>
#include <aws/apigateway/model/GetDocumentationVersionRequest.h>
#include <aws/apigateway/model/GetDocumentationVersionsRequest.h>
#include <aws/apigateway/model/GetDomainNameRequest.h>
#include <aws/apigateway/model/GetDomainNamesRequest.h>
#include <aws/apigateway/model/GetExportRequest.h>
#include <aws/apigateway/model/GetGatewayResponseRequest.h>
#include <aws/apigateway/model/GetGatewayResponsesRequest.h>
#include <aws/apigateway/model/GetIntegrationRequest.h>
#include <aws/apigateway/model/GetIntegrationResponseRequest.h>
#include <aws/apigateway/model/GetMethodRequest.h>
#include <aws/apigateway/model/GetMethodResponseRequest.h>
#include <aws/apigateway/model/GetModelRequest.h>
#include <aws/apigateway/model/GetModelTemplateRequest.h>
#include <aws/apigateway/model/GetModelsRequest.h>
#include <aws/apigateway/model/GetRequestValidatorRequest.h>
#include <aws/apigateway/model/GetRequestValidatorsRequest.h>
#include <aws/apigateway/model/GetResourceRequest.h>
#include <aws/apigateway/model/GetResourcesRequest.h>
#include <aws/apigateway/model/GetRestApiRequest.h>
#include <aws/apigateway/model/GetRestApisRequest.h>
#include <aws/apigateway/model/GetSdkRequest.h>
#include <aws/apigateway/model/GetSdkTypeRequest.h>
#include <aws/apigateway/model/GetSdkTypesRequest.h>
#include <aws/apigateway/model/GetStageRequest.h>
#include <aws/apigateway/model/GetStagesRequest.h>
#include <aws/apigateway/model/GetTagsRequest.h>
#include <aws/apigateway/model/GetUsageRequest.h>
#include <aws/apigateway/model/GetUsagePlanRequest.h>
#include <aws/apigateway/model/GetUsagePlanKeyRequest.h>
#include <aws/apigateway/model/GetUsagePlanKeysRequest.h>
#include <aws/apigateway/model/GetUsagePlansRequest.h>
#include <aws/apigateway/model/GetVpcLinkRequest.h>
#include <aws/apigateway/model/GetVpcLinksRequest.h>
#include <aws/apigateway/model/ImportApiKeysRequest.h>
#include <aws/apigateway/model/ImportDocumentationPartsRequest.h>
#include <aws/apigateway/model/ImportRestApiRequest.h>
#include <aws/apigateway/model/PutGatewayResponseRequest.h>
#include <aws/apigateway/model/PutIntegrationRequest.h>
#include <aws/apigateway/model/PutIntegrationResponseRequest.h>
#include <aws/apigateway/model/PutMethodRequest.h>
#include <aws/apigateway/model/PutMethodResponseRequest.h>
#include <aws/apigateway/model/PutRestApiRequest.h>
#include <aws/apigateway/model/TagResourceRequest.h>
#include <aws/apigateway/model/TestInvokeAuthorizerRequest.h>
#include <aws/apigateway/model/TestInvokeMethodRequest.h>
#include <aws/apigateway/model/UntagResourceRequest.h>
#include <aws/apigateway/model/UpdateAccountRequest.h>
#include <aws/apigateway/model/UpdateApiKeyRequest.h>
#include <aws/apigateway/model/UpdateAuthorizerRequest.h>
#include <aws/apigateway/model/UpdateBasePathMappingRequest.h>
#include <aws/apigateway/model/UpdateClientCertificateRequest.h>
#include <aws/apigateway/model/UpdateDeploymentRequest.h>
#include <aws/apigateway/model/UpdateDocumentationPartRequest.h>
#include <aws/apigateway/model/UpdateDocumentationVersionRequest.h>
#include <aws/apigateway/model/UpdateDomainNameRequest.h>
#include <aws/apigateway/model/UpdateGatewayResponseRequest.h>
#include <aws/apigateway/model/UpdateIntegrationRequest.h>
#include <aws/apigateway/model/UpdateIntegrationResponseRequest.h>
#include <aws/apigateway/model/UpdateMethodRequest.h>
#include <aws/apigateway/model/UpdateMethodResponseRequest.h>
#include <aws/apigateway/model/UpdateModelRequest.h>
#include <aws/apigateway/model/UpdateRequestValidatorRequest.h>
#include <aws/apigateway/model/UpdateResourceRequest.h>
#include <aws/apigateway/model/UpdateRestApiRequest.h>
#include <aws/apigateway/model/UpdateStageRequest.h>
#include <aws/apigateway/model/UpdateUsageRequest.h>
#include <aws/apigateway/model/UpdateUsagePlanRequest.h>
#include <aws/apigateway/model/UpdateVpcLinkRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::APIGateway;
using namespace Aws::APIGateway::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* APIGatewayClient::SERVICE_NAME = "apigateway";
const char* APIGatewayClient::ALLOCATION_TAG = "APIGatewayClient";

APIGatewayClient::APIGatewayClient(const APIGateway::APIGatewayClientConfiguration& clientConfiguration,
                                   std::shared_ptr<APIGatewayEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<APIGatewayErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

APIGatewayClient::APIGatewayClient(const AWSCredentials& credentials,
                                   std::shared_ptr<APIGatewayEndpointProviderBase> endpointProvider,
                                   const APIGateway::APIGatewayClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<APIGatewayErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

APIGatewayClient::APIGatewayClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                   std::shared_ptr<APIGatewayEndpointProviderBase> endpointProvider,
                                   const APIGateway::APIGatewayClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  credentialsProvider,
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<APIGatewayErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  APIGatewayClient::APIGatewayClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<APIGatewayErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<APIGatewayEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

APIGatewayClient::APIGatewayClient(const AWSCredentials& credentials,
                                   const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<APIGatewayErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<APIGatewayEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

APIGatewayClient::APIGatewayClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                   const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  credentialsProvider,
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<APIGatewayErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<APIGatewayEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
APIGatewayClient::~APIGatewayClient()
{
}

std::shared_ptr<APIGatewayEndpointProviderBase>& APIGatewayClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void APIGatewayClient::init(const APIGateway::APIGatewayClientConfiguration& config)
{
  AWSClient::SetServiceClientName("API Gateway");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void APIGatewayClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CreateApiKeyOutcome APIGatewayClient::CreateApiKey(const CreateApiKeyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateApiKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateApiKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/apikeys");
  return CreateApiKeyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateApiKeyOutcomeCallable APIGatewayClient::CreateApiKeyCallable(const CreateApiKeyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::CreateApiKey, this, request, m_executor.get());
}

void APIGatewayClient::CreateApiKeyAsync(const CreateApiKeyRequest& request, const CreateApiKeyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::CreateApiKey, this, request, handler, context, m_executor.get());
}

CreateAuthorizerOutcome APIGatewayClient::CreateAuthorizer(const CreateAuthorizerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateAuthorizer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateAuthorizer", "Required field: RestApiId, is not set");
    return CreateAuthorizerOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateAuthorizer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/authorizers");
  return CreateAuthorizerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateAuthorizerOutcomeCallable APIGatewayClient::CreateAuthorizerCallable(const CreateAuthorizerRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::CreateAuthorizer, this, request, m_executor.get());
}

void APIGatewayClient::CreateAuthorizerAsync(const CreateAuthorizerRequest& request, const CreateAuthorizerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::CreateAuthorizer, this, request, handler, context, m_executor.get());
}

CreateBasePathMappingOutcome APIGatewayClient::CreateBasePathMapping(const CreateBasePathMappingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateBasePathMapping, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DomainNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateBasePathMapping", "Required field: DomainName, is not set");
    return CreateBasePathMappingOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DomainName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateBasePathMapping, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/domainnames/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDomainName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/basepathmappings");
  return CreateBasePathMappingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateBasePathMappingOutcomeCallable APIGatewayClient::CreateBasePathMappingCallable(const CreateBasePathMappingRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::CreateBasePathMapping, this, request, m_executor.get());
}

void APIGatewayClient::CreateBasePathMappingAsync(const CreateBasePathMappingRequest& request, const CreateBasePathMappingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::CreateBasePathMapping, this, request, handler, context, m_executor.get());
}

CreateDeploymentOutcome APIGatewayClient::CreateDeployment(const CreateDeploymentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateDeployment", "Required field: RestApiId, is not set");
    return CreateDeploymentOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deployments");
  return CreateDeploymentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateDeploymentOutcomeCallable APIGatewayClient::CreateDeploymentCallable(const CreateDeploymentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::CreateDeployment, this, request, m_executor.get());
}

void APIGatewayClient::CreateDeploymentAsync(const CreateDeploymentRequest& request, const CreateDeploymentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::CreateDeployment, this, request, handler, context, m_executor.get());
}

CreateDocumentationPartOutcome APIGatewayClient::CreateDocumentationPart(const CreateDocumentationPartRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDocumentationPart, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateDocumentationPart", "Required field: RestApiId, is not set");
    return CreateDocumentationPartOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDocumentationPart, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/documentation/parts");
  return CreateDocumentationPartOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateDocumentationPartOutcomeCallable APIGatewayClient::CreateDocumentationPartCallable(const CreateDocumentationPartRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::CreateDocumentationPart, this, request, m_executor.get());
}

void APIGatewayClient::CreateDocumentationPartAsync(const CreateDocumentationPartRequest& request, const CreateDocumentationPartResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::CreateDocumentationPart, this, request, handler, context, m_executor.get());
}

CreateDocumentationVersionOutcome APIGatewayClient::CreateDocumentationVersion(const CreateDocumentationVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDocumentationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateDocumentationVersion", "Required field: RestApiId, is not set");
    return CreateDocumentationVersionOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDocumentationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/documentation/versions");
  return CreateDocumentationVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateDocumentationVersionOutcomeCallable APIGatewayClient::CreateDocumentationVersionCallable(const CreateDocumentationVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::CreateDocumentationVersion, this, request, m_executor.get());
}

void APIGatewayClient::CreateDocumentationVersionAsync(const CreateDocumentationVersionRequest& request, const CreateDocumentationVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::CreateDocumentationVersion, this, request, handler, context, m_executor.get());
}

CreateDomainNameOutcome APIGatewayClient::CreateDomainName(const CreateDomainNameRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/domainnames");
  return CreateDomainNameOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateDomainNameOutcomeCallable APIGatewayClient::CreateDomainNameCallable(const CreateDomainNameRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::CreateDomainName, this, request, m_executor.get());
}

void APIGatewayClient::CreateDomainNameAsync(const CreateDomainNameRequest& request, const CreateDomainNameResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::CreateDomainName, this, request, handler, context, m_executor.get());
}

CreateModelOutcome APIGatewayClient::CreateModel(const CreateModelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateModel", "Required field: RestApiId, is not set");
    return CreateModelOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/models");
  return CreateModelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateModelOutcomeCallable APIGatewayClient::CreateModelCallable(const CreateModelRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::CreateModel, this, request, m_executor.get());
}

void APIGatewayClient::CreateModelAsync(const CreateModelRequest& request, const CreateModelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::CreateModel, this, request, handler, context, m_executor.get());
}

CreateRequestValidatorOutcome APIGatewayClient::CreateRequestValidator(const CreateRequestValidatorRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateRequestValidator, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateRequestValidator", "Required field: RestApiId, is not set");
    return CreateRequestValidatorOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateRequestValidator, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/requestvalidators");
  return CreateRequestValidatorOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateRequestValidatorOutcomeCallable APIGatewayClient::CreateRequestValidatorCallable(const CreateRequestValidatorRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::CreateRequestValidator, this, request, m_executor.get());
}

void APIGatewayClient::CreateRequestValidatorAsync(const CreateRequestValidatorRequest& request, const CreateRequestValidatorResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::CreateRequestValidator, this, request, handler, context, m_executor.get());
}

CreateResourceOutcome APIGatewayClient::CreateResource(const CreateResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateResource", "Required field: RestApiId, is not set");
    return CreateResourceOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ParentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateResource", "Required field: ParentId, is not set");
    return CreateResourceOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ParentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetParentId());
  return CreateResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateResourceOutcomeCallable APIGatewayClient::CreateResourceCallable(const CreateResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::CreateResource, this, request, m_executor.get());
}

void APIGatewayClient::CreateResourceAsync(const CreateResourceRequest& request, const CreateResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::CreateResource, this, request, handler, context, m_executor.get());
}

CreateRestApiOutcome APIGatewayClient::CreateRestApi(const CreateRestApiRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateRestApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateRestApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis");
  return CreateRestApiOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateRestApiOutcomeCallable APIGatewayClient::CreateRestApiCallable(const CreateRestApiRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::CreateRestApi, this, request, m_executor.get());
}

void APIGatewayClient::CreateRestApiAsync(const CreateRestApiRequest& request, const CreateRestApiResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::CreateRestApi, this, request, handler, context, m_executor.get());
}

CreateStageOutcome APIGatewayClient::CreateStage(const CreateStageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateStage", "Required field: RestApiId, is not set");
    return CreateStageOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stages");
  return CreateStageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateStageOutcomeCallable APIGatewayClient::CreateStageCallable(const CreateStageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::CreateStage, this, request, m_executor.get());
}

void APIGatewayClient::CreateStageAsync(const CreateStageRequest& request, const CreateStageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::CreateStage, this, request, handler, context, m_executor.get());
}

CreateUsagePlanOutcome APIGatewayClient::CreateUsagePlan(const CreateUsagePlanRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateUsagePlan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateUsagePlan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/usageplans");
  return CreateUsagePlanOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateUsagePlanOutcomeCallable APIGatewayClient::CreateUsagePlanCallable(const CreateUsagePlanRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::CreateUsagePlan, this, request, m_executor.get());
}

void APIGatewayClient::CreateUsagePlanAsync(const CreateUsagePlanRequest& request, const CreateUsagePlanResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::CreateUsagePlan, this, request, handler, context, m_executor.get());
}

CreateUsagePlanKeyOutcome APIGatewayClient::CreateUsagePlanKey(const CreateUsagePlanKeyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateUsagePlanKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.UsagePlanIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateUsagePlanKey", "Required field: UsagePlanId, is not set");
    return CreateUsagePlanKeyOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [UsagePlanId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateUsagePlanKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/usageplans/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetUsagePlanId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/keys");
  return CreateUsagePlanKeyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateUsagePlanKeyOutcomeCallable APIGatewayClient::CreateUsagePlanKeyCallable(const CreateUsagePlanKeyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::CreateUsagePlanKey, this, request, m_executor.get());
}

void APIGatewayClient::CreateUsagePlanKeyAsync(const CreateUsagePlanKeyRequest& request, const CreateUsagePlanKeyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::CreateUsagePlanKey, this, request, handler, context, m_executor.get());
}

CreateVpcLinkOutcome APIGatewayClient::CreateVpcLink(const CreateVpcLinkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateVpcLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateVpcLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vpclinks");
  return CreateVpcLinkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateVpcLinkOutcomeCallable APIGatewayClient::CreateVpcLinkCallable(const CreateVpcLinkRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::CreateVpcLink, this, request, m_executor.get());
}

void APIGatewayClient::CreateVpcLinkAsync(const CreateVpcLinkRequest& request, const CreateVpcLinkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::CreateVpcLink, this, request, handler, context, m_executor.get());
}

DeleteApiKeyOutcome APIGatewayClient::DeleteApiKey(const DeleteApiKeyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteApiKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiKeyHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteApiKey", "Required field: ApiKey, is not set");
    return DeleteApiKeyOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiKey]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteApiKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/apikeys/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiKey());
  return DeleteApiKeyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteApiKeyOutcomeCallable APIGatewayClient::DeleteApiKeyCallable(const DeleteApiKeyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteApiKey, this, request, m_executor.get());
}

void APIGatewayClient::DeleteApiKeyAsync(const DeleteApiKeyRequest& request, const DeleteApiKeyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteApiKey, this, request, handler, context, m_executor.get());
}

DeleteAuthorizerOutcome APIGatewayClient::DeleteAuthorizer(const DeleteAuthorizerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteAuthorizer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteAuthorizer", "Required field: RestApiId, is not set");
    return DeleteAuthorizerOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.AuthorizerIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteAuthorizer", "Required field: AuthorizerId, is not set");
    return DeleteAuthorizerOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AuthorizerId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteAuthorizer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/authorizers/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAuthorizerId());
  return DeleteAuthorizerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteAuthorizerOutcomeCallable APIGatewayClient::DeleteAuthorizerCallable(const DeleteAuthorizerRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteAuthorizer, this, request, m_executor.get());
}

void APIGatewayClient::DeleteAuthorizerAsync(const DeleteAuthorizerRequest& request, const DeleteAuthorizerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteAuthorizer, this, request, handler, context, m_executor.get());
}

DeleteBasePathMappingOutcome APIGatewayClient::DeleteBasePathMapping(const DeleteBasePathMappingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteBasePathMapping, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DomainNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteBasePathMapping", "Required field: DomainName, is not set");
    return DeleteBasePathMappingOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DomainName]", false));
  }
  if (!request.BasePathHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteBasePathMapping", "Required field: BasePath, is not set");
    return DeleteBasePathMappingOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BasePath]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteBasePathMapping, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/domainnames/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDomainName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/basepathmappings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetBasePath());
  return DeleteBasePathMappingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteBasePathMappingOutcomeCallable APIGatewayClient::DeleteBasePathMappingCallable(const DeleteBasePathMappingRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteBasePathMapping, this, request, m_executor.get());
}

void APIGatewayClient::DeleteBasePathMappingAsync(const DeleteBasePathMappingRequest& request, const DeleteBasePathMappingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteBasePathMapping, this, request, handler, context, m_executor.get());
}

DeleteClientCertificateOutcome APIGatewayClient::DeleteClientCertificate(const DeleteClientCertificateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteClientCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClientCertificateIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteClientCertificate", "Required field: ClientCertificateId, is not set");
    return DeleteClientCertificateOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClientCertificateId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteClientCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clientcertificates/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClientCertificateId());
  return DeleteClientCertificateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteClientCertificateOutcomeCallable APIGatewayClient::DeleteClientCertificateCallable(const DeleteClientCertificateRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteClientCertificate, this, request, m_executor.get());
}

void APIGatewayClient::DeleteClientCertificateAsync(const DeleteClientCertificateRequest& request, const DeleteClientCertificateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteClientCertificate, this, request, handler, context, m_executor.get());
}

DeleteDeploymentOutcome APIGatewayClient::DeleteDeployment(const DeleteDeploymentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDeployment", "Required field: RestApiId, is not set");
    return DeleteDeploymentOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.DeploymentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDeployment", "Required field: DeploymentId, is not set");
    return DeleteDeploymentOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DeploymentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deployments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDeploymentId());
  return DeleteDeploymentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteDeploymentOutcomeCallable APIGatewayClient::DeleteDeploymentCallable(const DeleteDeploymentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteDeployment, this, request, m_executor.get());
}

void APIGatewayClient::DeleteDeploymentAsync(const DeleteDeploymentRequest& request, const DeleteDeploymentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteDeployment, this, request, handler, context, m_executor.get());
}

DeleteDocumentationPartOutcome APIGatewayClient::DeleteDocumentationPart(const DeleteDocumentationPartRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDocumentationPart, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDocumentationPart", "Required field: RestApiId, is not set");
    return DeleteDocumentationPartOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.DocumentationPartIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDocumentationPart", "Required field: DocumentationPartId, is not set");
    return DeleteDocumentationPartOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DocumentationPartId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDocumentationPart, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/documentation/parts/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDocumentationPartId());
  return DeleteDocumentationPartOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteDocumentationPartOutcomeCallable APIGatewayClient::DeleteDocumentationPartCallable(const DeleteDocumentationPartRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteDocumentationPart, this, request, m_executor.get());
}

void APIGatewayClient::DeleteDocumentationPartAsync(const DeleteDocumentationPartRequest& request, const DeleteDocumentationPartResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteDocumentationPart, this, request, handler, context, m_executor.get());
}

DeleteDocumentationVersionOutcome APIGatewayClient::DeleteDocumentationVersion(const DeleteDocumentationVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDocumentationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDocumentationVersion", "Required field: RestApiId, is not set");
    return DeleteDocumentationVersionOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.DocumentationVersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDocumentationVersion", "Required field: DocumentationVersion, is not set");
    return DeleteDocumentationVersionOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DocumentationVersion]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDocumentationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/documentation/versions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDocumentationVersion());
  return DeleteDocumentationVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteDocumentationVersionOutcomeCallable APIGatewayClient::DeleteDocumentationVersionCallable(const DeleteDocumentationVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteDocumentationVersion, this, request, m_executor.get());
}

void APIGatewayClient::DeleteDocumentationVersionAsync(const DeleteDocumentationVersionRequest& request, const DeleteDocumentationVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteDocumentationVersion, this, request, handler, context, m_executor.get());
}

DeleteDomainNameOutcome APIGatewayClient::DeleteDomainName(const DeleteDomainNameRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DomainNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDomainName", "Required field: DomainName, is not set");
    return DeleteDomainNameOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DomainName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/domainnames/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDomainName());
  return DeleteDomainNameOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteDomainNameOutcomeCallable APIGatewayClient::DeleteDomainNameCallable(const DeleteDomainNameRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteDomainName, this, request, m_executor.get());
}

void APIGatewayClient::DeleteDomainNameAsync(const DeleteDomainNameRequest& request, const DeleteDomainNameResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteDomainName, this, request, handler, context, m_executor.get());
}

DeleteGatewayResponseOutcome APIGatewayClient::DeleteGatewayResponse(const DeleteGatewayResponseRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteGatewayResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteGatewayResponse", "Required field: RestApiId, is not set");
    return DeleteGatewayResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResponseTypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteGatewayResponse", "Required field: ResponseType, is not set");
    return DeleteGatewayResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResponseType]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteGatewayResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/gatewayresponses/");
  endpointResolutionOutcome.GetResult().AddPathSegment(GatewayResponseTypeMapper::GetNameForGatewayResponseType(request.GetResponseType()));
  return DeleteGatewayResponseOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteGatewayResponseOutcomeCallable APIGatewayClient::DeleteGatewayResponseCallable(const DeleteGatewayResponseRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteGatewayResponse, this, request, m_executor.get());
}

void APIGatewayClient::DeleteGatewayResponseAsync(const DeleteGatewayResponseRequest& request, const DeleteGatewayResponseResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteGatewayResponse, this, request, handler, context, m_executor.get());
}

DeleteIntegrationOutcome APIGatewayClient::DeleteIntegration(const DeleteIntegrationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteIntegration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteIntegration", "Required field: RestApiId, is not set");
    return DeleteIntegrationOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteIntegration", "Required field: ResourceId, is not set");
    return DeleteIntegrationOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteIntegration", "Required field: HttpMethod, is not set");
    return DeleteIntegrationOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteIntegration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  endpointResolutionOutcome.GetResult().AddPathSegments("/integration");
  return DeleteIntegrationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteIntegrationOutcomeCallable APIGatewayClient::DeleteIntegrationCallable(const DeleteIntegrationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteIntegration, this, request, m_executor.get());
}

void APIGatewayClient::DeleteIntegrationAsync(const DeleteIntegrationRequest& request, const DeleteIntegrationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteIntegration, this, request, handler, context, m_executor.get());
}

DeleteIntegrationResponseOutcome APIGatewayClient::DeleteIntegrationResponse(const DeleteIntegrationResponseRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteIntegrationResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteIntegrationResponse", "Required field: RestApiId, is not set");
    return DeleteIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteIntegrationResponse", "Required field: ResourceId, is not set");
    return DeleteIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteIntegrationResponse", "Required field: HttpMethod, is not set");
    return DeleteIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  if (!request.StatusCodeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteIntegrationResponse", "Required field: StatusCode, is not set");
    return DeleteIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StatusCode]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteIntegrationResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  endpointResolutionOutcome.GetResult().AddPathSegments("/integration/responses/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStatusCode());
  return DeleteIntegrationResponseOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteIntegrationResponseOutcomeCallable APIGatewayClient::DeleteIntegrationResponseCallable(const DeleteIntegrationResponseRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteIntegrationResponse, this, request, m_executor.get());
}

void APIGatewayClient::DeleteIntegrationResponseAsync(const DeleteIntegrationResponseRequest& request, const DeleteIntegrationResponseResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteIntegrationResponse, this, request, handler, context, m_executor.get());
}

DeleteMethodOutcome APIGatewayClient::DeleteMethod(const DeleteMethodRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteMethod, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteMethod", "Required field: RestApiId, is not set");
    return DeleteMethodOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteMethod", "Required field: ResourceId, is not set");
    return DeleteMethodOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteMethod", "Required field: HttpMethod, is not set");
    return DeleteMethodOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteMethod, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  return DeleteMethodOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteMethodOutcomeCallable APIGatewayClient::DeleteMethodCallable(const DeleteMethodRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteMethod, this, request, m_executor.get());
}

void APIGatewayClient::DeleteMethodAsync(const DeleteMethodRequest& request, const DeleteMethodResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteMethod, this, request, handler, context, m_executor.get());
}

DeleteMethodResponseOutcome APIGatewayClient::DeleteMethodResponse(const DeleteMethodResponseRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteMethodResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteMethodResponse", "Required field: RestApiId, is not set");
    return DeleteMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteMethodResponse", "Required field: ResourceId, is not set");
    return DeleteMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteMethodResponse", "Required field: HttpMethod, is not set");
    return DeleteMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  if (!request.StatusCodeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteMethodResponse", "Required field: StatusCode, is not set");
    return DeleteMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StatusCode]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteMethodResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  endpointResolutionOutcome.GetResult().AddPathSegments("/responses/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStatusCode());
  return DeleteMethodResponseOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteMethodResponseOutcomeCallable APIGatewayClient::DeleteMethodResponseCallable(const DeleteMethodResponseRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteMethodResponse, this, request, m_executor.get());
}

void APIGatewayClient::DeleteMethodResponseAsync(const DeleteMethodResponseRequest& request, const DeleteMethodResponseResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteMethodResponse, this, request, handler, context, m_executor.get());
}

DeleteModelOutcome APIGatewayClient::DeleteModel(const DeleteModelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteModel", "Required field: RestApiId, is not set");
    return DeleteModelOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ModelNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteModel", "Required field: ModelName, is not set");
    return DeleteModelOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ModelName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/models/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetModelName());
  return DeleteModelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteModelOutcomeCallable APIGatewayClient::DeleteModelCallable(const DeleteModelRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteModel, this, request, m_executor.get());
}

void APIGatewayClient::DeleteModelAsync(const DeleteModelRequest& request, const DeleteModelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteModel, this, request, handler, context, m_executor.get());
}

DeleteRequestValidatorOutcome APIGatewayClient::DeleteRequestValidator(const DeleteRequestValidatorRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteRequestValidator, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteRequestValidator", "Required field: RestApiId, is not set");
    return DeleteRequestValidatorOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.RequestValidatorIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteRequestValidator", "Required field: RequestValidatorId, is not set");
    return DeleteRequestValidatorOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RequestValidatorId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteRequestValidator, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/requestvalidators/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRequestValidatorId());
  return DeleteRequestValidatorOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteRequestValidatorOutcomeCallable APIGatewayClient::DeleteRequestValidatorCallable(const DeleteRequestValidatorRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteRequestValidator, this, request, m_executor.get());
}

void APIGatewayClient::DeleteRequestValidatorAsync(const DeleteRequestValidatorRequest& request, const DeleteRequestValidatorResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteRequestValidator, this, request, handler, context, m_executor.get());
}

DeleteResourceOutcome APIGatewayClient::DeleteResource(const DeleteResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteResource", "Required field: RestApiId, is not set");
    return DeleteResourceOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteResource", "Required field: ResourceId, is not set");
    return DeleteResourceOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  return DeleteResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteResourceOutcomeCallable APIGatewayClient::DeleteResourceCallable(const DeleteResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteResource, this, request, m_executor.get());
}

void APIGatewayClient::DeleteResourceAsync(const DeleteResourceRequest& request, const DeleteResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteResource, this, request, handler, context, m_executor.get());
}

DeleteRestApiOutcome APIGatewayClient::DeleteRestApi(const DeleteRestApiRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteRestApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteRestApi", "Required field: RestApiId, is not set");
    return DeleteRestApiOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteRestApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  return DeleteRestApiOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteRestApiOutcomeCallable APIGatewayClient::DeleteRestApiCallable(const DeleteRestApiRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteRestApi, this, request, m_executor.get());
}

void APIGatewayClient::DeleteRestApiAsync(const DeleteRestApiRequest& request, const DeleteRestApiResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteRestApi, this, request, handler, context, m_executor.get());
}

DeleteStageOutcome APIGatewayClient::DeleteStage(const DeleteStageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteStage", "Required field: RestApiId, is not set");
    return DeleteStageOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.StageNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteStage", "Required field: StageName, is not set");
    return DeleteStageOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StageName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stages/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStageName());
  return DeleteStageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteStageOutcomeCallable APIGatewayClient::DeleteStageCallable(const DeleteStageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteStage, this, request, m_executor.get());
}

void APIGatewayClient::DeleteStageAsync(const DeleteStageRequest& request, const DeleteStageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteStage, this, request, handler, context, m_executor.get());
}

DeleteUsagePlanOutcome APIGatewayClient::DeleteUsagePlan(const DeleteUsagePlanRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteUsagePlan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.UsagePlanIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteUsagePlan", "Required field: UsagePlanId, is not set");
    return DeleteUsagePlanOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [UsagePlanId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteUsagePlan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/usageplans/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetUsagePlanId());
  return DeleteUsagePlanOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteUsagePlanOutcomeCallable APIGatewayClient::DeleteUsagePlanCallable(const DeleteUsagePlanRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteUsagePlan, this, request, m_executor.get());
}

void APIGatewayClient::DeleteUsagePlanAsync(const DeleteUsagePlanRequest& request, const DeleteUsagePlanResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteUsagePlan, this, request, handler, context, m_executor.get());
}

DeleteUsagePlanKeyOutcome APIGatewayClient::DeleteUsagePlanKey(const DeleteUsagePlanKeyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteUsagePlanKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.UsagePlanIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteUsagePlanKey", "Required field: UsagePlanId, is not set");
    return DeleteUsagePlanKeyOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [UsagePlanId]", false));
  }
  if (!request.KeyIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteUsagePlanKey", "Required field: KeyId, is not set");
    return DeleteUsagePlanKeyOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [KeyId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteUsagePlanKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/usageplans/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetUsagePlanId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/keys/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetKeyId());
  return DeleteUsagePlanKeyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteUsagePlanKeyOutcomeCallable APIGatewayClient::DeleteUsagePlanKeyCallable(const DeleteUsagePlanKeyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteUsagePlanKey, this, request, m_executor.get());
}

void APIGatewayClient::DeleteUsagePlanKeyAsync(const DeleteUsagePlanKeyRequest& request, const DeleteUsagePlanKeyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteUsagePlanKey, this, request, handler, context, m_executor.get());
}

DeleteVpcLinkOutcome APIGatewayClient::DeleteVpcLink(const DeleteVpcLinkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteVpcLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.VpcLinkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteVpcLink", "Required field: VpcLinkId, is not set");
    return DeleteVpcLinkOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VpcLinkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteVpcLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vpclinks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVpcLinkId());
  return DeleteVpcLinkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteVpcLinkOutcomeCallable APIGatewayClient::DeleteVpcLinkCallable(const DeleteVpcLinkRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::DeleteVpcLink, this, request, m_executor.get());
}

void APIGatewayClient::DeleteVpcLinkAsync(const DeleteVpcLinkRequest& request, const DeleteVpcLinkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::DeleteVpcLink, this, request, handler, context, m_executor.get());
}

FlushStageAuthorizersCacheOutcome APIGatewayClient::FlushStageAuthorizersCache(const FlushStageAuthorizersCacheRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, FlushStageAuthorizersCache, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("FlushStageAuthorizersCache", "Required field: RestApiId, is not set");
    return FlushStageAuthorizersCacheOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.StageNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("FlushStageAuthorizersCache", "Required field: StageName, is not set");
    return FlushStageAuthorizersCacheOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StageName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, FlushStageAuthorizersCache, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stages/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStageName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/cache/authorizers");
  return FlushStageAuthorizersCacheOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

FlushStageAuthorizersCacheOutcomeCallable APIGatewayClient::FlushStageAuthorizersCacheCallable(const FlushStageAuthorizersCacheRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::FlushStageAuthorizersCache, this, request, m_executor.get());
}

void APIGatewayClient::FlushStageAuthorizersCacheAsync(const FlushStageAuthorizersCacheRequest& request, const FlushStageAuthorizersCacheResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::FlushStageAuthorizersCache, this, request, handler, context, m_executor.get());
}

FlushStageCacheOutcome APIGatewayClient::FlushStageCache(const FlushStageCacheRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, FlushStageCache, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("FlushStageCache", "Required field: RestApiId, is not set");
    return FlushStageCacheOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.StageNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("FlushStageCache", "Required field: StageName, is not set");
    return FlushStageCacheOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StageName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, FlushStageCache, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stages/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStageName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/cache/data");
  return FlushStageCacheOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

FlushStageCacheOutcomeCallable APIGatewayClient::FlushStageCacheCallable(const FlushStageCacheRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::FlushStageCache, this, request, m_executor.get());
}

void APIGatewayClient::FlushStageCacheAsync(const FlushStageCacheRequest& request, const FlushStageCacheResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::FlushStageCache, this, request, handler, context, m_executor.get());
}

GenerateClientCertificateOutcome APIGatewayClient::GenerateClientCertificate(const GenerateClientCertificateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GenerateClientCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GenerateClientCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clientcertificates");
  return GenerateClientCertificateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GenerateClientCertificateOutcomeCallable APIGatewayClient::GenerateClientCertificateCallable(const GenerateClientCertificateRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GenerateClientCertificate, this, request, m_executor.get());
}

void APIGatewayClient::GenerateClientCertificateAsync(const GenerateClientCertificateRequest& request, const GenerateClientCertificateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GenerateClientCertificate, this, request, handler, context, m_executor.get());
}

GetAccountOutcome APIGatewayClient::GetAccount(const GetAccountRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/account");
  return GetAccountOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetAccountOutcomeCallable APIGatewayClient::GetAccountCallable(const GetAccountRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetAccount, this, request, m_executor.get());
}

void APIGatewayClient::GetAccountAsync(const GetAccountRequest& request, const GetAccountResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetAccount, this, request, handler, context, m_executor.get());
}

GetApiKeyOutcome APIGatewayClient::GetApiKey(const GetApiKeyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetApiKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiKeyHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetApiKey", "Required field: ApiKey, is not set");
    return GetApiKeyOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiKey]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetApiKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/apikeys/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiKey());
  return GetApiKeyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetApiKeyOutcomeCallable APIGatewayClient::GetApiKeyCallable(const GetApiKeyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetApiKey, this, request, m_executor.get());
}

void APIGatewayClient::GetApiKeyAsync(const GetApiKeyRequest& request, const GetApiKeyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetApiKey, this, request, handler, context, m_executor.get());
}

GetApiKeysOutcome APIGatewayClient::GetApiKeys(const GetApiKeysRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetApiKeys, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetApiKeys, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/apikeys");
  return GetApiKeysOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetApiKeysOutcomeCallable APIGatewayClient::GetApiKeysCallable(const GetApiKeysRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetApiKeys, this, request, m_executor.get());
}

void APIGatewayClient::GetApiKeysAsync(const GetApiKeysRequest& request, const GetApiKeysResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetApiKeys, this, request, handler, context, m_executor.get());
}

GetAuthorizerOutcome APIGatewayClient::GetAuthorizer(const GetAuthorizerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetAuthorizer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetAuthorizer", "Required field: RestApiId, is not set");
    return GetAuthorizerOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.AuthorizerIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetAuthorizer", "Required field: AuthorizerId, is not set");
    return GetAuthorizerOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AuthorizerId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetAuthorizer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/authorizers/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAuthorizerId());
  return GetAuthorizerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetAuthorizerOutcomeCallable APIGatewayClient::GetAuthorizerCallable(const GetAuthorizerRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetAuthorizer, this, request, m_executor.get());
}

void APIGatewayClient::GetAuthorizerAsync(const GetAuthorizerRequest& request, const GetAuthorizerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetAuthorizer, this, request, handler, context, m_executor.get());
}

GetAuthorizersOutcome APIGatewayClient::GetAuthorizers(const GetAuthorizersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetAuthorizers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetAuthorizers", "Required field: RestApiId, is not set");
    return GetAuthorizersOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetAuthorizers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/authorizers");
  return GetAuthorizersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetAuthorizersOutcomeCallable APIGatewayClient::GetAuthorizersCallable(const GetAuthorizersRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetAuthorizers, this, request, m_executor.get());
}

void APIGatewayClient::GetAuthorizersAsync(const GetAuthorizersRequest& request, const GetAuthorizersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetAuthorizers, this, request, handler, context, m_executor.get());
}

GetBasePathMappingOutcome APIGatewayClient::GetBasePathMapping(const GetBasePathMappingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetBasePathMapping, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DomainNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetBasePathMapping", "Required field: DomainName, is not set");
    return GetBasePathMappingOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DomainName]", false));
  }
  if (!request.BasePathHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetBasePathMapping", "Required field: BasePath, is not set");
    return GetBasePathMappingOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BasePath]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetBasePathMapping, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/domainnames/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDomainName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/basepathmappings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetBasePath());
  return GetBasePathMappingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetBasePathMappingOutcomeCallable APIGatewayClient::GetBasePathMappingCallable(const GetBasePathMappingRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetBasePathMapping, this, request, m_executor.get());
}

void APIGatewayClient::GetBasePathMappingAsync(const GetBasePathMappingRequest& request, const GetBasePathMappingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetBasePathMapping, this, request, handler, context, m_executor.get());
}

GetBasePathMappingsOutcome APIGatewayClient::GetBasePathMappings(const GetBasePathMappingsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetBasePathMappings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DomainNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetBasePathMappings", "Required field: DomainName, is not set");
    return GetBasePathMappingsOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DomainName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetBasePathMappings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/domainnames/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDomainName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/basepathmappings");
  return GetBasePathMappingsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetBasePathMappingsOutcomeCallable APIGatewayClient::GetBasePathMappingsCallable(const GetBasePathMappingsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetBasePathMappings, this, request, m_executor.get());
}

void APIGatewayClient::GetBasePathMappingsAsync(const GetBasePathMappingsRequest& request, const GetBasePathMappingsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetBasePathMappings, this, request, handler, context, m_executor.get());
}

GetClientCertificateOutcome APIGatewayClient::GetClientCertificate(const GetClientCertificateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetClientCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClientCertificateIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetClientCertificate", "Required field: ClientCertificateId, is not set");
    return GetClientCertificateOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClientCertificateId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetClientCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clientcertificates/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClientCertificateId());
  return GetClientCertificateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetClientCertificateOutcomeCallable APIGatewayClient::GetClientCertificateCallable(const GetClientCertificateRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetClientCertificate, this, request, m_executor.get());
}

void APIGatewayClient::GetClientCertificateAsync(const GetClientCertificateRequest& request, const GetClientCertificateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetClientCertificate, this, request, handler, context, m_executor.get());
}

GetClientCertificatesOutcome APIGatewayClient::GetClientCertificates(const GetClientCertificatesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetClientCertificates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetClientCertificates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clientcertificates");
  return GetClientCertificatesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetClientCertificatesOutcomeCallable APIGatewayClient::GetClientCertificatesCallable(const GetClientCertificatesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetClientCertificates, this, request, m_executor.get());
}

void APIGatewayClient::GetClientCertificatesAsync(const GetClientCertificatesRequest& request, const GetClientCertificatesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetClientCertificates, this, request, handler, context, m_executor.get());
}

GetDeploymentOutcome APIGatewayClient::GetDeployment(const GetDeploymentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDeployment", "Required field: RestApiId, is not set");
    return GetDeploymentOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.DeploymentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDeployment", "Required field: DeploymentId, is not set");
    return GetDeploymentOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DeploymentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deployments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDeploymentId());
  return GetDeploymentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetDeploymentOutcomeCallable APIGatewayClient::GetDeploymentCallable(const GetDeploymentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetDeployment, this, request, m_executor.get());
}

void APIGatewayClient::GetDeploymentAsync(const GetDeploymentRequest& request, const GetDeploymentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetDeployment, this, request, handler, context, m_executor.get());
}

GetDeploymentsOutcome APIGatewayClient::GetDeployments(const GetDeploymentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDeployments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDeployments", "Required field: RestApiId, is not set");
    return GetDeploymentsOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDeployments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deployments");
  return GetDeploymentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetDeploymentsOutcomeCallable APIGatewayClient::GetDeploymentsCallable(const GetDeploymentsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetDeployments, this, request, m_executor.get());
}

void APIGatewayClient::GetDeploymentsAsync(const GetDeploymentsRequest& request, const GetDeploymentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetDeployments, this, request, handler, context, m_executor.get());
}

GetDocumentationPartOutcome APIGatewayClient::GetDocumentationPart(const GetDocumentationPartRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDocumentationPart, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDocumentationPart", "Required field: RestApiId, is not set");
    return GetDocumentationPartOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.DocumentationPartIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDocumentationPart", "Required field: DocumentationPartId, is not set");
    return GetDocumentationPartOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DocumentationPartId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDocumentationPart, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/documentation/parts/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDocumentationPartId());
  return GetDocumentationPartOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetDocumentationPartOutcomeCallable APIGatewayClient::GetDocumentationPartCallable(const GetDocumentationPartRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetDocumentationPart, this, request, m_executor.get());
}

void APIGatewayClient::GetDocumentationPartAsync(const GetDocumentationPartRequest& request, const GetDocumentationPartResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetDocumentationPart, this, request, handler, context, m_executor.get());
}

GetDocumentationPartsOutcome APIGatewayClient::GetDocumentationParts(const GetDocumentationPartsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDocumentationParts, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDocumentationParts", "Required field: RestApiId, is not set");
    return GetDocumentationPartsOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDocumentationParts, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/documentation/parts");
  return GetDocumentationPartsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetDocumentationPartsOutcomeCallable APIGatewayClient::GetDocumentationPartsCallable(const GetDocumentationPartsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetDocumentationParts, this, request, m_executor.get());
}

void APIGatewayClient::GetDocumentationPartsAsync(const GetDocumentationPartsRequest& request, const GetDocumentationPartsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetDocumentationParts, this, request, handler, context, m_executor.get());
}

GetDocumentationVersionOutcome APIGatewayClient::GetDocumentationVersion(const GetDocumentationVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDocumentationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDocumentationVersion", "Required field: RestApiId, is not set");
    return GetDocumentationVersionOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.DocumentationVersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDocumentationVersion", "Required field: DocumentationVersion, is not set");
    return GetDocumentationVersionOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DocumentationVersion]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDocumentationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/documentation/versions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDocumentationVersion());
  return GetDocumentationVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetDocumentationVersionOutcomeCallable APIGatewayClient::GetDocumentationVersionCallable(const GetDocumentationVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetDocumentationVersion, this, request, m_executor.get());
}

void APIGatewayClient::GetDocumentationVersionAsync(const GetDocumentationVersionRequest& request, const GetDocumentationVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetDocumentationVersion, this, request, handler, context, m_executor.get());
}

GetDocumentationVersionsOutcome APIGatewayClient::GetDocumentationVersions(const GetDocumentationVersionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDocumentationVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDocumentationVersions", "Required field: RestApiId, is not set");
    return GetDocumentationVersionsOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDocumentationVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/documentation/versions");
  return GetDocumentationVersionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetDocumentationVersionsOutcomeCallable APIGatewayClient::GetDocumentationVersionsCallable(const GetDocumentationVersionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetDocumentationVersions, this, request, m_executor.get());
}

void APIGatewayClient::GetDocumentationVersionsAsync(const GetDocumentationVersionsRequest& request, const GetDocumentationVersionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetDocumentationVersions, this, request, handler, context, m_executor.get());
}

GetDomainNameOutcome APIGatewayClient::GetDomainName(const GetDomainNameRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DomainNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDomainName", "Required field: DomainName, is not set");
    return GetDomainNameOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DomainName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/domainnames/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDomainName());
  return GetDomainNameOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetDomainNameOutcomeCallable APIGatewayClient::GetDomainNameCallable(const GetDomainNameRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetDomainName, this, request, m_executor.get());
}

void APIGatewayClient::GetDomainNameAsync(const GetDomainNameRequest& request, const GetDomainNameResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetDomainName, this, request, handler, context, m_executor.get());
}

GetDomainNamesOutcome APIGatewayClient::GetDomainNames(const GetDomainNamesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDomainNames, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDomainNames, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/domainnames");
  return GetDomainNamesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetDomainNamesOutcomeCallable APIGatewayClient::GetDomainNamesCallable(const GetDomainNamesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetDomainNames, this, request, m_executor.get());
}

void APIGatewayClient::GetDomainNamesAsync(const GetDomainNamesRequest& request, const GetDomainNamesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetDomainNames, this, request, handler, context, m_executor.get());
}

GetExportOutcome APIGatewayClient::GetExport(const GetExportRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetExport, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExport", "Required field: RestApiId, is not set");
    return GetExportOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.StageNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExport", "Required field: StageName, is not set");
    return GetExportOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StageName]", false));
  }
  if (!request.ExportTypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExport", "Required field: ExportType, is not set");
    return GetExportOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ExportType]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetExport, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stages/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStageName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/exports/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetExportType());
  return GetExportOutcome(MakeRequestWithUnparsedResponse(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET));
}

GetExportOutcomeCallable APIGatewayClient::GetExportCallable(const GetExportRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetExport, this, request, m_executor.get());
}

void APIGatewayClient::GetExportAsync(const GetExportRequest& request, const GetExportResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetExport, this, request, handler, context, m_executor.get());
}

GetGatewayResponseOutcome APIGatewayClient::GetGatewayResponse(const GetGatewayResponseRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetGatewayResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetGatewayResponse", "Required field: RestApiId, is not set");
    return GetGatewayResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResponseTypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetGatewayResponse", "Required field: ResponseType, is not set");
    return GetGatewayResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResponseType]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetGatewayResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/gatewayresponses/");
  endpointResolutionOutcome.GetResult().AddPathSegment(GatewayResponseTypeMapper::GetNameForGatewayResponseType(request.GetResponseType()));
  return GetGatewayResponseOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetGatewayResponseOutcomeCallable APIGatewayClient::GetGatewayResponseCallable(const GetGatewayResponseRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetGatewayResponse, this, request, m_executor.get());
}

void APIGatewayClient::GetGatewayResponseAsync(const GetGatewayResponseRequest& request, const GetGatewayResponseResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetGatewayResponse, this, request, handler, context, m_executor.get());
}

GetGatewayResponsesOutcome APIGatewayClient::GetGatewayResponses(const GetGatewayResponsesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetGatewayResponses, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetGatewayResponses", "Required field: RestApiId, is not set");
    return GetGatewayResponsesOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetGatewayResponses, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/gatewayresponses");
  return GetGatewayResponsesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetGatewayResponsesOutcomeCallable APIGatewayClient::GetGatewayResponsesCallable(const GetGatewayResponsesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetGatewayResponses, this, request, m_executor.get());
}

void APIGatewayClient::GetGatewayResponsesAsync(const GetGatewayResponsesRequest& request, const GetGatewayResponsesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetGatewayResponses, this, request, handler, context, m_executor.get());
}

GetIntegrationOutcome APIGatewayClient::GetIntegration(const GetIntegrationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetIntegration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetIntegration", "Required field: RestApiId, is not set");
    return GetIntegrationOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetIntegration", "Required field: ResourceId, is not set");
    return GetIntegrationOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetIntegration", "Required field: HttpMethod, is not set");
    return GetIntegrationOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetIntegration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  endpointResolutionOutcome.GetResult().AddPathSegments("/integration");
  return GetIntegrationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetIntegrationOutcomeCallable APIGatewayClient::GetIntegrationCallable(const GetIntegrationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetIntegration, this, request, m_executor.get());
}

void APIGatewayClient::GetIntegrationAsync(const GetIntegrationRequest& request, const GetIntegrationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetIntegration, this, request, handler, context, m_executor.get());
}

GetIntegrationResponseOutcome APIGatewayClient::GetIntegrationResponse(const GetIntegrationResponseRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetIntegrationResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetIntegrationResponse", "Required field: RestApiId, is not set");
    return GetIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetIntegrationResponse", "Required field: ResourceId, is not set");
    return GetIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetIntegrationResponse", "Required field: HttpMethod, is not set");
    return GetIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  if (!request.StatusCodeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetIntegrationResponse", "Required field: StatusCode, is not set");
    return GetIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StatusCode]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetIntegrationResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  endpointResolutionOutcome.GetResult().AddPathSegments("/integration/responses/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStatusCode());
  return GetIntegrationResponseOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetIntegrationResponseOutcomeCallable APIGatewayClient::GetIntegrationResponseCallable(const GetIntegrationResponseRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetIntegrationResponse, this, request, m_executor.get());
}

void APIGatewayClient::GetIntegrationResponseAsync(const GetIntegrationResponseRequest& request, const GetIntegrationResponseResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetIntegrationResponse, this, request, handler, context, m_executor.get());
}

GetMethodOutcome APIGatewayClient::GetMethod(const GetMethodRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetMethod, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetMethod", "Required field: RestApiId, is not set");
    return GetMethodOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetMethod", "Required field: ResourceId, is not set");
    return GetMethodOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetMethod", "Required field: HttpMethod, is not set");
    return GetMethodOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetMethod, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  return GetMethodOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetMethodOutcomeCallable APIGatewayClient::GetMethodCallable(const GetMethodRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetMethod, this, request, m_executor.get());
}

void APIGatewayClient::GetMethodAsync(const GetMethodRequest& request, const GetMethodResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetMethod, this, request, handler, context, m_executor.get());
}

GetMethodResponseOutcome APIGatewayClient::GetMethodResponse(const GetMethodResponseRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetMethodResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetMethodResponse", "Required field: RestApiId, is not set");
    return GetMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetMethodResponse", "Required field: ResourceId, is not set");
    return GetMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetMethodResponse", "Required field: HttpMethod, is not set");
    return GetMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  if (!request.StatusCodeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetMethodResponse", "Required field: StatusCode, is not set");
    return GetMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StatusCode]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetMethodResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  endpointResolutionOutcome.GetResult().AddPathSegments("/responses/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStatusCode());
  return GetMethodResponseOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetMethodResponseOutcomeCallable APIGatewayClient::GetMethodResponseCallable(const GetMethodResponseRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetMethodResponse, this, request, m_executor.get());
}

void APIGatewayClient::GetMethodResponseAsync(const GetMethodResponseRequest& request, const GetMethodResponseResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetMethodResponse, this, request, handler, context, m_executor.get());
}

GetModelOutcome APIGatewayClient::GetModel(const GetModelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetModel", "Required field: RestApiId, is not set");
    return GetModelOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ModelNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetModel", "Required field: ModelName, is not set");
    return GetModelOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ModelName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/models/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetModelName());
  return GetModelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetModelOutcomeCallable APIGatewayClient::GetModelCallable(const GetModelRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetModel, this, request, m_executor.get());
}

void APIGatewayClient::GetModelAsync(const GetModelRequest& request, const GetModelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetModel, this, request, handler, context, m_executor.get());
}

GetModelTemplateOutcome APIGatewayClient::GetModelTemplate(const GetModelTemplateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetModelTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetModelTemplate", "Required field: RestApiId, is not set");
    return GetModelTemplateOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ModelNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetModelTemplate", "Required field: ModelName, is not set");
    return GetModelTemplateOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ModelName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetModelTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/models/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetModelName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/default_template");
  return GetModelTemplateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetModelTemplateOutcomeCallable APIGatewayClient::GetModelTemplateCallable(const GetModelTemplateRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetModelTemplate, this, request, m_executor.get());
}

void APIGatewayClient::GetModelTemplateAsync(const GetModelTemplateRequest& request, const GetModelTemplateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetModelTemplate, this, request, handler, context, m_executor.get());
}

GetModelsOutcome APIGatewayClient::GetModels(const GetModelsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetModels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetModels", "Required field: RestApiId, is not set");
    return GetModelsOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetModels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/models");
  return GetModelsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetModelsOutcomeCallable APIGatewayClient::GetModelsCallable(const GetModelsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetModels, this, request, m_executor.get());
}

void APIGatewayClient::GetModelsAsync(const GetModelsRequest& request, const GetModelsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetModels, this, request, handler, context, m_executor.get());
}

GetRequestValidatorOutcome APIGatewayClient::GetRequestValidator(const GetRequestValidatorRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRequestValidator, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetRequestValidator", "Required field: RestApiId, is not set");
    return GetRequestValidatorOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.RequestValidatorIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetRequestValidator", "Required field: RequestValidatorId, is not set");
    return GetRequestValidatorOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RequestValidatorId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRequestValidator, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/requestvalidators/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRequestValidatorId());
  return GetRequestValidatorOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetRequestValidatorOutcomeCallable APIGatewayClient::GetRequestValidatorCallable(const GetRequestValidatorRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetRequestValidator, this, request, m_executor.get());
}

void APIGatewayClient::GetRequestValidatorAsync(const GetRequestValidatorRequest& request, const GetRequestValidatorResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetRequestValidator, this, request, handler, context, m_executor.get());
}

GetRequestValidatorsOutcome APIGatewayClient::GetRequestValidators(const GetRequestValidatorsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRequestValidators, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetRequestValidators", "Required field: RestApiId, is not set");
    return GetRequestValidatorsOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRequestValidators, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/requestvalidators");
  return GetRequestValidatorsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetRequestValidatorsOutcomeCallable APIGatewayClient::GetRequestValidatorsCallable(const GetRequestValidatorsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetRequestValidators, this, request, m_executor.get());
}

void APIGatewayClient::GetRequestValidatorsAsync(const GetRequestValidatorsRequest& request, const GetRequestValidatorsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetRequestValidators, this, request, handler, context, m_executor.get());
}

GetResourceOutcome APIGatewayClient::GetResource(const GetResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetResource", "Required field: RestApiId, is not set");
    return GetResourceOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetResource", "Required field: ResourceId, is not set");
    return GetResourceOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  return GetResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetResourceOutcomeCallable APIGatewayClient::GetResourceCallable(const GetResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetResource, this, request, m_executor.get());
}

void APIGatewayClient::GetResourceAsync(const GetResourceRequest& request, const GetResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetResource, this, request, handler, context, m_executor.get());
}

GetResourcesOutcome APIGatewayClient::GetResources(const GetResourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetResources", "Required field: RestApiId, is not set");
    return GetResourcesOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources");
  return GetResourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetResourcesOutcomeCallable APIGatewayClient::GetResourcesCallable(const GetResourcesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetResources, this, request, m_executor.get());
}

void APIGatewayClient::GetResourcesAsync(const GetResourcesRequest& request, const GetResourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetResources, this, request, handler, context, m_executor.get());
}

GetRestApiOutcome APIGatewayClient::GetRestApi(const GetRestApiRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRestApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetRestApi", "Required field: RestApiId, is not set");
    return GetRestApiOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRestApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  return GetRestApiOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetRestApiOutcomeCallable APIGatewayClient::GetRestApiCallable(const GetRestApiRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetRestApi, this, request, m_executor.get());
}

void APIGatewayClient::GetRestApiAsync(const GetRestApiRequest& request, const GetRestApiResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetRestApi, this, request, handler, context, m_executor.get());
}

GetRestApisOutcome APIGatewayClient::GetRestApis(const GetRestApisRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRestApis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRestApis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis");
  return GetRestApisOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetRestApisOutcomeCallable APIGatewayClient::GetRestApisCallable(const GetRestApisRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetRestApis, this, request, m_executor.get());
}

void APIGatewayClient::GetRestApisAsync(const GetRestApisRequest& request, const GetRestApisResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetRestApis, this, request, handler, context, m_executor.get());
}

GetSdkOutcome APIGatewayClient::GetSdk(const GetSdkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSdk, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetSdk", "Required field: RestApiId, is not set");
    return GetSdkOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.StageNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetSdk", "Required field: StageName, is not set");
    return GetSdkOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StageName]", false));
  }
  if (!request.SdkTypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetSdk", "Required field: SdkType, is not set");
    return GetSdkOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SdkType]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSdk, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stages/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStageName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sdks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetSdkType());
  return GetSdkOutcome(MakeRequestWithUnparsedResponse(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET));
}

GetSdkOutcomeCallable APIGatewayClient::GetSdkCallable(const GetSdkRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetSdk, this, request, m_executor.get());
}

void APIGatewayClient::GetSdkAsync(const GetSdkRequest& request, const GetSdkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetSdk, this, request, handler, context, m_executor.get());
}

GetSdkTypeOutcome APIGatewayClient::GetSdkType(const GetSdkTypeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSdkType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetSdkType", "Required field: Id, is not set");
    return GetSdkTypeOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSdkType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sdktypes/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return GetSdkTypeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetSdkTypeOutcomeCallable APIGatewayClient::GetSdkTypeCallable(const GetSdkTypeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetSdkType, this, request, m_executor.get());
}

void APIGatewayClient::GetSdkTypeAsync(const GetSdkTypeRequest& request, const GetSdkTypeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetSdkType, this, request, handler, context, m_executor.get());
}

GetSdkTypesOutcome APIGatewayClient::GetSdkTypes(const GetSdkTypesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSdkTypes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSdkTypes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sdktypes");
  return GetSdkTypesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetSdkTypesOutcomeCallable APIGatewayClient::GetSdkTypesCallable(const GetSdkTypesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetSdkTypes, this, request, m_executor.get());
}

void APIGatewayClient::GetSdkTypesAsync(const GetSdkTypesRequest& request, const GetSdkTypesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetSdkTypes, this, request, handler, context, m_executor.get());
}

GetStageOutcome APIGatewayClient::GetStage(const GetStageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetStage", "Required field: RestApiId, is not set");
    return GetStageOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.StageNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetStage", "Required field: StageName, is not set");
    return GetStageOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StageName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stages/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStageName());
  return GetStageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetStageOutcomeCallable APIGatewayClient::GetStageCallable(const GetStageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetStage, this, request, m_executor.get());
}

void APIGatewayClient::GetStageAsync(const GetStageRequest& request, const GetStageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetStage, this, request, handler, context, m_executor.get());
}

GetStagesOutcome APIGatewayClient::GetStages(const GetStagesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetStages, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetStages", "Required field: RestApiId, is not set");
    return GetStagesOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetStages, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stages");
  return GetStagesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetStagesOutcomeCallable APIGatewayClient::GetStagesCallable(const GetStagesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetStages, this, request, m_executor.get());
}

void APIGatewayClient::GetStagesAsync(const GetStagesRequest& request, const GetStagesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetStages, this, request, handler, context, m_executor.get());
}

GetTagsOutcome APIGatewayClient::GetTags(const GetTagsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetTags", "Required field: ResourceArn, is not set");
    return GetTagsOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return GetTagsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetTagsOutcomeCallable APIGatewayClient::GetTagsCallable(const GetTagsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetTags, this, request, m_executor.get());
}

void APIGatewayClient::GetTagsAsync(const GetTagsRequest& request, const GetTagsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetTags, this, request, handler, context, m_executor.get());
}

GetUsageOutcome APIGatewayClient::GetUsage(const GetUsageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetUsage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.UsagePlanIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetUsage", "Required field: UsagePlanId, is not set");
    return GetUsageOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [UsagePlanId]", false));
  }
  if (!request.StartDateHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetUsage", "Required field: StartDate, is not set");
    return GetUsageOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StartDate]", false));
  }
  if (!request.EndDateHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetUsage", "Required field: EndDate, is not set");
    return GetUsageOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EndDate]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetUsage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/usageplans/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetUsagePlanId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/usage");
  return GetUsageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetUsageOutcomeCallable APIGatewayClient::GetUsageCallable(const GetUsageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetUsage, this, request, m_executor.get());
}

void APIGatewayClient::GetUsageAsync(const GetUsageRequest& request, const GetUsageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetUsage, this, request, handler, context, m_executor.get());
}

GetUsagePlanOutcome APIGatewayClient::GetUsagePlan(const GetUsagePlanRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetUsagePlan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.UsagePlanIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetUsagePlan", "Required field: UsagePlanId, is not set");
    return GetUsagePlanOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [UsagePlanId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetUsagePlan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/usageplans/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetUsagePlanId());
  return GetUsagePlanOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetUsagePlanOutcomeCallable APIGatewayClient::GetUsagePlanCallable(const GetUsagePlanRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetUsagePlan, this, request, m_executor.get());
}

void APIGatewayClient::GetUsagePlanAsync(const GetUsagePlanRequest& request, const GetUsagePlanResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetUsagePlan, this, request, handler, context, m_executor.get());
}

GetUsagePlanKeyOutcome APIGatewayClient::GetUsagePlanKey(const GetUsagePlanKeyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetUsagePlanKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.UsagePlanIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetUsagePlanKey", "Required field: UsagePlanId, is not set");
    return GetUsagePlanKeyOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [UsagePlanId]", false));
  }
  if (!request.KeyIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetUsagePlanKey", "Required field: KeyId, is not set");
    return GetUsagePlanKeyOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [KeyId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetUsagePlanKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/usageplans/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetUsagePlanId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/keys/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetKeyId());
  return GetUsagePlanKeyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetUsagePlanKeyOutcomeCallable APIGatewayClient::GetUsagePlanKeyCallable(const GetUsagePlanKeyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetUsagePlanKey, this, request, m_executor.get());
}

void APIGatewayClient::GetUsagePlanKeyAsync(const GetUsagePlanKeyRequest& request, const GetUsagePlanKeyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetUsagePlanKey, this, request, handler, context, m_executor.get());
}

GetUsagePlanKeysOutcome APIGatewayClient::GetUsagePlanKeys(const GetUsagePlanKeysRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetUsagePlanKeys, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.UsagePlanIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetUsagePlanKeys", "Required field: UsagePlanId, is not set");
    return GetUsagePlanKeysOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [UsagePlanId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetUsagePlanKeys, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/usageplans/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetUsagePlanId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/keys");
  return GetUsagePlanKeysOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetUsagePlanKeysOutcomeCallable APIGatewayClient::GetUsagePlanKeysCallable(const GetUsagePlanKeysRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetUsagePlanKeys, this, request, m_executor.get());
}

void APIGatewayClient::GetUsagePlanKeysAsync(const GetUsagePlanKeysRequest& request, const GetUsagePlanKeysResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetUsagePlanKeys, this, request, handler, context, m_executor.get());
}

GetUsagePlansOutcome APIGatewayClient::GetUsagePlans(const GetUsagePlansRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetUsagePlans, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetUsagePlans, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/usageplans");
  return GetUsagePlansOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetUsagePlansOutcomeCallable APIGatewayClient::GetUsagePlansCallable(const GetUsagePlansRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetUsagePlans, this, request, m_executor.get());
}

void APIGatewayClient::GetUsagePlansAsync(const GetUsagePlansRequest& request, const GetUsagePlansResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetUsagePlans, this, request, handler, context, m_executor.get());
}

GetVpcLinkOutcome APIGatewayClient::GetVpcLink(const GetVpcLinkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetVpcLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.VpcLinkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetVpcLink", "Required field: VpcLinkId, is not set");
    return GetVpcLinkOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VpcLinkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetVpcLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vpclinks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVpcLinkId());
  return GetVpcLinkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetVpcLinkOutcomeCallable APIGatewayClient::GetVpcLinkCallable(const GetVpcLinkRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetVpcLink, this, request, m_executor.get());
}

void APIGatewayClient::GetVpcLinkAsync(const GetVpcLinkRequest& request, const GetVpcLinkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetVpcLink, this, request, handler, context, m_executor.get());
}

GetVpcLinksOutcome APIGatewayClient::GetVpcLinks(const GetVpcLinksRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetVpcLinks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetVpcLinks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vpclinks");
  return GetVpcLinksOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetVpcLinksOutcomeCallable APIGatewayClient::GetVpcLinksCallable(const GetVpcLinksRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::GetVpcLinks, this, request, m_executor.get());
}

void APIGatewayClient::GetVpcLinksAsync(const GetVpcLinksRequest& request, const GetVpcLinksResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::GetVpcLinks, this, request, handler, context, m_executor.get());
}

ImportApiKeysOutcome APIGatewayClient::ImportApiKeys(const ImportApiKeysRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ImportApiKeys, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.FormatHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ImportApiKeys", "Required field: Format, is not set");
    return ImportApiKeysOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Format]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ImportApiKeys, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/apikeys");
  ss.str("?mode=import");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return ImportApiKeysOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ImportApiKeysOutcomeCallable APIGatewayClient::ImportApiKeysCallable(const ImportApiKeysRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::ImportApiKeys, this, request, m_executor.get());
}

void APIGatewayClient::ImportApiKeysAsync(const ImportApiKeysRequest& request, const ImportApiKeysResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::ImportApiKeys, this, request, handler, context, m_executor.get());
}

ImportDocumentationPartsOutcome APIGatewayClient::ImportDocumentationParts(const ImportDocumentationPartsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ImportDocumentationParts, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ImportDocumentationParts", "Required field: RestApiId, is not set");
    return ImportDocumentationPartsOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ImportDocumentationParts, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/documentation/parts");
  return ImportDocumentationPartsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

ImportDocumentationPartsOutcomeCallable APIGatewayClient::ImportDocumentationPartsCallable(const ImportDocumentationPartsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::ImportDocumentationParts, this, request, m_executor.get());
}

void APIGatewayClient::ImportDocumentationPartsAsync(const ImportDocumentationPartsRequest& request, const ImportDocumentationPartsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::ImportDocumentationParts, this, request, handler, context, m_executor.get());
}

ImportRestApiOutcome APIGatewayClient::ImportRestApi(const ImportRestApiRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ImportRestApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ImportRestApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis");
  ss.str("?mode=import");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return ImportRestApiOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ImportRestApiOutcomeCallable APIGatewayClient::ImportRestApiCallable(const ImportRestApiRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::ImportRestApi, this, request, m_executor.get());
}

void APIGatewayClient::ImportRestApiAsync(const ImportRestApiRequest& request, const ImportRestApiResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::ImportRestApi, this, request, handler, context, m_executor.get());
}

PutGatewayResponseOutcome APIGatewayClient::PutGatewayResponse(const PutGatewayResponseRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutGatewayResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutGatewayResponse", "Required field: RestApiId, is not set");
    return PutGatewayResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResponseTypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutGatewayResponse", "Required field: ResponseType, is not set");
    return PutGatewayResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResponseType]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutGatewayResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/gatewayresponses/");
  endpointResolutionOutcome.GetResult().AddPathSegment(GatewayResponseTypeMapper::GetNameForGatewayResponseType(request.GetResponseType()));
  return PutGatewayResponseOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PutGatewayResponseOutcomeCallable APIGatewayClient::PutGatewayResponseCallable(const PutGatewayResponseRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::PutGatewayResponse, this, request, m_executor.get());
}

void APIGatewayClient::PutGatewayResponseAsync(const PutGatewayResponseRequest& request, const PutGatewayResponseResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::PutGatewayResponse, this, request, handler, context, m_executor.get());
}

PutIntegrationOutcome APIGatewayClient::PutIntegration(const PutIntegrationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutIntegration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutIntegration", "Required field: RestApiId, is not set");
    return PutIntegrationOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutIntegration", "Required field: ResourceId, is not set");
    return PutIntegrationOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutIntegration", "Required field: HttpMethod, is not set");
    return PutIntegrationOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutIntegration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  endpointResolutionOutcome.GetResult().AddPathSegments("/integration");
  return PutIntegrationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PutIntegrationOutcomeCallable APIGatewayClient::PutIntegrationCallable(const PutIntegrationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::PutIntegration, this, request, m_executor.get());
}

void APIGatewayClient::PutIntegrationAsync(const PutIntegrationRequest& request, const PutIntegrationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::PutIntegration, this, request, handler, context, m_executor.get());
}

PutIntegrationResponseOutcome APIGatewayClient::PutIntegrationResponse(const PutIntegrationResponseRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutIntegrationResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutIntegrationResponse", "Required field: RestApiId, is not set");
    return PutIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutIntegrationResponse", "Required field: ResourceId, is not set");
    return PutIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutIntegrationResponse", "Required field: HttpMethod, is not set");
    return PutIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  if (!request.StatusCodeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutIntegrationResponse", "Required field: StatusCode, is not set");
    return PutIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StatusCode]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutIntegrationResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  endpointResolutionOutcome.GetResult().AddPathSegments("/integration/responses/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStatusCode());
  return PutIntegrationResponseOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PutIntegrationResponseOutcomeCallable APIGatewayClient::PutIntegrationResponseCallable(const PutIntegrationResponseRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::PutIntegrationResponse, this, request, m_executor.get());
}

void APIGatewayClient::PutIntegrationResponseAsync(const PutIntegrationResponseRequest& request, const PutIntegrationResponseResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::PutIntegrationResponse, this, request, handler, context, m_executor.get());
}

PutMethodOutcome APIGatewayClient::PutMethod(const PutMethodRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutMethod, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutMethod", "Required field: RestApiId, is not set");
    return PutMethodOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutMethod", "Required field: ResourceId, is not set");
    return PutMethodOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutMethod", "Required field: HttpMethod, is not set");
    return PutMethodOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutMethod, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  return PutMethodOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PutMethodOutcomeCallable APIGatewayClient::PutMethodCallable(const PutMethodRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::PutMethod, this, request, m_executor.get());
}

void APIGatewayClient::PutMethodAsync(const PutMethodRequest& request, const PutMethodResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::PutMethod, this, request, handler, context, m_executor.get());
}

PutMethodResponseOutcome APIGatewayClient::PutMethodResponse(const PutMethodResponseRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutMethodResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutMethodResponse", "Required field: RestApiId, is not set");
    return PutMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutMethodResponse", "Required field: ResourceId, is not set");
    return PutMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutMethodResponse", "Required field: HttpMethod, is not set");
    return PutMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  if (!request.StatusCodeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutMethodResponse", "Required field: StatusCode, is not set");
    return PutMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StatusCode]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutMethodResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  endpointResolutionOutcome.GetResult().AddPathSegments("/responses/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStatusCode());
  return PutMethodResponseOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PutMethodResponseOutcomeCallable APIGatewayClient::PutMethodResponseCallable(const PutMethodResponseRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::PutMethodResponse, this, request, m_executor.get());
}

void APIGatewayClient::PutMethodResponseAsync(const PutMethodResponseRequest& request, const PutMethodResponseResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::PutMethodResponse, this, request, handler, context, m_executor.get());
}

PutRestApiOutcome APIGatewayClient::PutRestApi(const PutRestApiRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutRestApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutRestApi", "Required field: RestApiId, is not set");
    return PutRestApiOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutRestApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  return PutRestApiOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PutRestApiOutcomeCallable APIGatewayClient::PutRestApiCallable(const PutRestApiRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::PutRestApi, this, request, m_executor.get());
}

void APIGatewayClient::PutRestApiAsync(const PutRestApiRequest& request, const PutRestApiResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::PutRestApi, this, request, handler, context, m_executor.get());
}

TagResourceOutcome APIGatewayClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable APIGatewayClient::TagResourceCallable(const TagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::TagResource, this, request, m_executor.get());
}

void APIGatewayClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::TagResource, this, request, handler, context, m_executor.get());
}

TestInvokeAuthorizerOutcome APIGatewayClient::TestInvokeAuthorizer(const TestInvokeAuthorizerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TestInvokeAuthorizer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TestInvokeAuthorizer", "Required field: RestApiId, is not set");
    return TestInvokeAuthorizerOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.AuthorizerIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TestInvokeAuthorizer", "Required field: AuthorizerId, is not set");
    return TestInvokeAuthorizerOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AuthorizerId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TestInvokeAuthorizer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/authorizers/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAuthorizerId());
  return TestInvokeAuthorizerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TestInvokeAuthorizerOutcomeCallable APIGatewayClient::TestInvokeAuthorizerCallable(const TestInvokeAuthorizerRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::TestInvokeAuthorizer, this, request, m_executor.get());
}

void APIGatewayClient::TestInvokeAuthorizerAsync(const TestInvokeAuthorizerRequest& request, const TestInvokeAuthorizerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::TestInvokeAuthorizer, this, request, handler, context, m_executor.get());
}

TestInvokeMethodOutcome APIGatewayClient::TestInvokeMethod(const TestInvokeMethodRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TestInvokeMethod, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TestInvokeMethod", "Required field: RestApiId, is not set");
    return TestInvokeMethodOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TestInvokeMethod", "Required field: ResourceId, is not set");
    return TestInvokeMethodOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TestInvokeMethod", "Required field: HttpMethod, is not set");
    return TestInvokeMethodOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TestInvokeMethod, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  return TestInvokeMethodOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TestInvokeMethodOutcomeCallable APIGatewayClient::TestInvokeMethodCallable(const TestInvokeMethodRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::TestInvokeMethod, this, request, m_executor.get());
}

void APIGatewayClient::TestInvokeMethodAsync(const TestInvokeMethodRequest& request, const TestInvokeMethodResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::TestInvokeMethod, this, request, handler, context, m_executor.get());
}

UntagResourceOutcome APIGatewayClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable APIGatewayClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UntagResource, this, request, m_executor.get());
}

void APIGatewayClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UntagResource, this, request, handler, context, m_executor.get());
}

UpdateAccountOutcome APIGatewayClient::UpdateAccount(const UpdateAccountRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/account");
  return UpdateAccountOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateAccountOutcomeCallable APIGatewayClient::UpdateAccountCallable(const UpdateAccountRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateAccount, this, request, m_executor.get());
}

void APIGatewayClient::UpdateAccountAsync(const UpdateAccountRequest& request, const UpdateAccountResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateAccount, this, request, handler, context, m_executor.get());
}

UpdateApiKeyOutcome APIGatewayClient::UpdateApiKey(const UpdateApiKeyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateApiKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiKeyHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateApiKey", "Required field: ApiKey, is not set");
    return UpdateApiKeyOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiKey]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateApiKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/apikeys/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiKey());
  return UpdateApiKeyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateApiKeyOutcomeCallable APIGatewayClient::UpdateApiKeyCallable(const UpdateApiKeyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateApiKey, this, request, m_executor.get());
}

void APIGatewayClient::UpdateApiKeyAsync(const UpdateApiKeyRequest& request, const UpdateApiKeyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateApiKey, this, request, handler, context, m_executor.get());
}

UpdateAuthorizerOutcome APIGatewayClient::UpdateAuthorizer(const UpdateAuthorizerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateAuthorizer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateAuthorizer", "Required field: RestApiId, is not set");
    return UpdateAuthorizerOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.AuthorizerIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateAuthorizer", "Required field: AuthorizerId, is not set");
    return UpdateAuthorizerOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AuthorizerId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateAuthorizer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/authorizers/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAuthorizerId());
  return UpdateAuthorizerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateAuthorizerOutcomeCallable APIGatewayClient::UpdateAuthorizerCallable(const UpdateAuthorizerRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateAuthorizer, this, request, m_executor.get());
}

void APIGatewayClient::UpdateAuthorizerAsync(const UpdateAuthorizerRequest& request, const UpdateAuthorizerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateAuthorizer, this, request, handler, context, m_executor.get());
}

UpdateBasePathMappingOutcome APIGatewayClient::UpdateBasePathMapping(const UpdateBasePathMappingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateBasePathMapping, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DomainNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateBasePathMapping", "Required field: DomainName, is not set");
    return UpdateBasePathMappingOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DomainName]", false));
  }
  if (!request.BasePathHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateBasePathMapping", "Required field: BasePath, is not set");
    return UpdateBasePathMappingOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BasePath]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateBasePathMapping, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/domainnames/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDomainName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/basepathmappings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetBasePath());
  return UpdateBasePathMappingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateBasePathMappingOutcomeCallable APIGatewayClient::UpdateBasePathMappingCallable(const UpdateBasePathMappingRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateBasePathMapping, this, request, m_executor.get());
}

void APIGatewayClient::UpdateBasePathMappingAsync(const UpdateBasePathMappingRequest& request, const UpdateBasePathMappingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateBasePathMapping, this, request, handler, context, m_executor.get());
}

UpdateClientCertificateOutcome APIGatewayClient::UpdateClientCertificate(const UpdateClientCertificateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateClientCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClientCertificateIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateClientCertificate", "Required field: ClientCertificateId, is not set");
    return UpdateClientCertificateOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClientCertificateId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateClientCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clientcertificates/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClientCertificateId());
  return UpdateClientCertificateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateClientCertificateOutcomeCallable APIGatewayClient::UpdateClientCertificateCallable(const UpdateClientCertificateRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateClientCertificate, this, request, m_executor.get());
}

void APIGatewayClient::UpdateClientCertificateAsync(const UpdateClientCertificateRequest& request, const UpdateClientCertificateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateClientCertificate, this, request, handler, context, m_executor.get());
}

UpdateDeploymentOutcome APIGatewayClient::UpdateDeployment(const UpdateDeploymentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDeployment", "Required field: RestApiId, is not set");
    return UpdateDeploymentOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.DeploymentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDeployment", "Required field: DeploymentId, is not set");
    return UpdateDeploymentOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DeploymentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deployments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDeploymentId());
  return UpdateDeploymentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateDeploymentOutcomeCallable APIGatewayClient::UpdateDeploymentCallable(const UpdateDeploymentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateDeployment, this, request, m_executor.get());
}

void APIGatewayClient::UpdateDeploymentAsync(const UpdateDeploymentRequest& request, const UpdateDeploymentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateDeployment, this, request, handler, context, m_executor.get());
}

UpdateDocumentationPartOutcome APIGatewayClient::UpdateDocumentationPart(const UpdateDocumentationPartRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateDocumentationPart, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDocumentationPart", "Required field: RestApiId, is not set");
    return UpdateDocumentationPartOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.DocumentationPartIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDocumentationPart", "Required field: DocumentationPartId, is not set");
    return UpdateDocumentationPartOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DocumentationPartId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateDocumentationPart, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/documentation/parts/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDocumentationPartId());
  return UpdateDocumentationPartOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateDocumentationPartOutcomeCallable APIGatewayClient::UpdateDocumentationPartCallable(const UpdateDocumentationPartRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateDocumentationPart, this, request, m_executor.get());
}

void APIGatewayClient::UpdateDocumentationPartAsync(const UpdateDocumentationPartRequest& request, const UpdateDocumentationPartResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateDocumentationPart, this, request, handler, context, m_executor.get());
}

UpdateDocumentationVersionOutcome APIGatewayClient::UpdateDocumentationVersion(const UpdateDocumentationVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateDocumentationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDocumentationVersion", "Required field: RestApiId, is not set");
    return UpdateDocumentationVersionOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.DocumentationVersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDocumentationVersion", "Required field: DocumentationVersion, is not set");
    return UpdateDocumentationVersionOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DocumentationVersion]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateDocumentationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/documentation/versions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDocumentationVersion());
  return UpdateDocumentationVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateDocumentationVersionOutcomeCallable APIGatewayClient::UpdateDocumentationVersionCallable(const UpdateDocumentationVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateDocumentationVersion, this, request, m_executor.get());
}

void APIGatewayClient::UpdateDocumentationVersionAsync(const UpdateDocumentationVersionRequest& request, const UpdateDocumentationVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateDocumentationVersion, this, request, handler, context, m_executor.get());
}

UpdateDomainNameOutcome APIGatewayClient::UpdateDomainName(const UpdateDomainNameRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DomainNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDomainName", "Required field: DomainName, is not set");
    return UpdateDomainNameOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DomainName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/domainnames/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDomainName());
  return UpdateDomainNameOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateDomainNameOutcomeCallable APIGatewayClient::UpdateDomainNameCallable(const UpdateDomainNameRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateDomainName, this, request, m_executor.get());
}

void APIGatewayClient::UpdateDomainNameAsync(const UpdateDomainNameRequest& request, const UpdateDomainNameResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateDomainName, this, request, handler, context, m_executor.get());
}

UpdateGatewayResponseOutcome APIGatewayClient::UpdateGatewayResponse(const UpdateGatewayResponseRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateGatewayResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateGatewayResponse", "Required field: RestApiId, is not set");
    return UpdateGatewayResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResponseTypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateGatewayResponse", "Required field: ResponseType, is not set");
    return UpdateGatewayResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResponseType]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateGatewayResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/gatewayresponses/");
  endpointResolutionOutcome.GetResult().AddPathSegment(GatewayResponseTypeMapper::GetNameForGatewayResponseType(request.GetResponseType()));
  return UpdateGatewayResponseOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateGatewayResponseOutcomeCallable APIGatewayClient::UpdateGatewayResponseCallable(const UpdateGatewayResponseRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateGatewayResponse, this, request, m_executor.get());
}

void APIGatewayClient::UpdateGatewayResponseAsync(const UpdateGatewayResponseRequest& request, const UpdateGatewayResponseResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateGatewayResponse, this, request, handler, context, m_executor.get());
}

UpdateIntegrationOutcome APIGatewayClient::UpdateIntegration(const UpdateIntegrationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateIntegration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateIntegration", "Required field: RestApiId, is not set");
    return UpdateIntegrationOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateIntegration", "Required field: ResourceId, is not set");
    return UpdateIntegrationOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateIntegration", "Required field: HttpMethod, is not set");
    return UpdateIntegrationOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateIntegration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  endpointResolutionOutcome.GetResult().AddPathSegments("/integration");
  return UpdateIntegrationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateIntegrationOutcomeCallable APIGatewayClient::UpdateIntegrationCallable(const UpdateIntegrationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateIntegration, this, request, m_executor.get());
}

void APIGatewayClient::UpdateIntegrationAsync(const UpdateIntegrationRequest& request, const UpdateIntegrationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateIntegration, this, request, handler, context, m_executor.get());
}

UpdateIntegrationResponseOutcome APIGatewayClient::UpdateIntegrationResponse(const UpdateIntegrationResponseRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateIntegrationResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateIntegrationResponse", "Required field: RestApiId, is not set");
    return UpdateIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateIntegrationResponse", "Required field: ResourceId, is not set");
    return UpdateIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateIntegrationResponse", "Required field: HttpMethod, is not set");
    return UpdateIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  if (!request.StatusCodeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateIntegrationResponse", "Required field: StatusCode, is not set");
    return UpdateIntegrationResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StatusCode]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateIntegrationResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  endpointResolutionOutcome.GetResult().AddPathSegments("/integration/responses/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStatusCode());
  return UpdateIntegrationResponseOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateIntegrationResponseOutcomeCallable APIGatewayClient::UpdateIntegrationResponseCallable(const UpdateIntegrationResponseRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateIntegrationResponse, this, request, m_executor.get());
}

void APIGatewayClient::UpdateIntegrationResponseAsync(const UpdateIntegrationResponseRequest& request, const UpdateIntegrationResponseResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateIntegrationResponse, this, request, handler, context, m_executor.get());
}

UpdateMethodOutcome APIGatewayClient::UpdateMethod(const UpdateMethodRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateMethod, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateMethod", "Required field: RestApiId, is not set");
    return UpdateMethodOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateMethod", "Required field: ResourceId, is not set");
    return UpdateMethodOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateMethod", "Required field: HttpMethod, is not set");
    return UpdateMethodOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateMethod, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  return UpdateMethodOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateMethodOutcomeCallable APIGatewayClient::UpdateMethodCallable(const UpdateMethodRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateMethod, this, request, m_executor.get());
}

void APIGatewayClient::UpdateMethodAsync(const UpdateMethodRequest& request, const UpdateMethodResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateMethod, this, request, handler, context, m_executor.get());
}

UpdateMethodResponseOutcome APIGatewayClient::UpdateMethodResponse(const UpdateMethodResponseRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateMethodResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateMethodResponse", "Required field: RestApiId, is not set");
    return UpdateMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateMethodResponse", "Required field: ResourceId, is not set");
    return UpdateMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  if (!request.HttpMethodHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateMethodResponse", "Required field: HttpMethod, is not set");
    return UpdateMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [HttpMethod]", false));
  }
  if (!request.StatusCodeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateMethodResponse", "Required field: StatusCode, is not set");
    return UpdateMethodResponseOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StatusCode]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateMethodResponse, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/methods/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetHttpMethod());
  endpointResolutionOutcome.GetResult().AddPathSegments("/responses/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStatusCode());
  return UpdateMethodResponseOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateMethodResponseOutcomeCallable APIGatewayClient::UpdateMethodResponseCallable(const UpdateMethodResponseRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateMethodResponse, this, request, m_executor.get());
}

void APIGatewayClient::UpdateMethodResponseAsync(const UpdateMethodResponseRequest& request, const UpdateMethodResponseResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateMethodResponse, this, request, handler, context, m_executor.get());
}

UpdateModelOutcome APIGatewayClient::UpdateModel(const UpdateModelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateModel", "Required field: RestApiId, is not set");
    return UpdateModelOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ModelNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateModel", "Required field: ModelName, is not set");
    return UpdateModelOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ModelName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/models/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetModelName());
  return UpdateModelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateModelOutcomeCallable APIGatewayClient::UpdateModelCallable(const UpdateModelRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateModel, this, request, m_executor.get());
}

void APIGatewayClient::UpdateModelAsync(const UpdateModelRequest& request, const UpdateModelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateModel, this, request, handler, context, m_executor.get());
}

UpdateRequestValidatorOutcome APIGatewayClient::UpdateRequestValidator(const UpdateRequestValidatorRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateRequestValidator, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateRequestValidator", "Required field: RestApiId, is not set");
    return UpdateRequestValidatorOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.RequestValidatorIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateRequestValidator", "Required field: RequestValidatorId, is not set");
    return UpdateRequestValidatorOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RequestValidatorId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateRequestValidator, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/requestvalidators/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRequestValidatorId());
  return UpdateRequestValidatorOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateRequestValidatorOutcomeCallable APIGatewayClient::UpdateRequestValidatorCallable(const UpdateRequestValidatorRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateRequestValidator, this, request, m_executor.get());
}

void APIGatewayClient::UpdateRequestValidatorAsync(const UpdateRequestValidatorRequest& request, const UpdateRequestValidatorResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateRequestValidator, this, request, handler, context, m_executor.get());
}

UpdateResourceOutcome APIGatewayClient::UpdateResource(const UpdateResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateResource", "Required field: RestApiId, is not set");
    return UpdateResourceOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.ResourceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateResource", "Required field: ResourceId, is not set");
    return UpdateResourceOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceId());
  return UpdateResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateResourceOutcomeCallable APIGatewayClient::UpdateResourceCallable(const UpdateResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateResource, this, request, m_executor.get());
}

void APIGatewayClient::UpdateResourceAsync(const UpdateResourceRequest& request, const UpdateResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateResource, this, request, handler, context, m_executor.get());
}

UpdateRestApiOutcome APIGatewayClient::UpdateRestApi(const UpdateRestApiRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateRestApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateRestApi", "Required field: RestApiId, is not set");
    return UpdateRestApiOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateRestApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  return UpdateRestApiOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateRestApiOutcomeCallable APIGatewayClient::UpdateRestApiCallable(const UpdateRestApiRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateRestApi, this, request, m_executor.get());
}

void APIGatewayClient::UpdateRestApiAsync(const UpdateRestApiRequest& request, const UpdateRestApiResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateRestApi, this, request, handler, context, m_executor.get());
}

UpdateStageOutcome APIGatewayClient::UpdateStage(const UpdateStageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RestApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateStage", "Required field: RestApiId, is not set");
    return UpdateStageOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RestApiId]", false));
  }
  if (!request.StageNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateStage", "Required field: StageName, is not set");
    return UpdateStageOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StageName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restapis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRestApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stages/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStageName());
  return UpdateStageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateStageOutcomeCallable APIGatewayClient::UpdateStageCallable(const UpdateStageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateStage, this, request, m_executor.get());
}

void APIGatewayClient::UpdateStageAsync(const UpdateStageRequest& request, const UpdateStageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateStage, this, request, handler, context, m_executor.get());
}

UpdateUsageOutcome APIGatewayClient::UpdateUsage(const UpdateUsageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateUsage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.UsagePlanIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateUsage", "Required field: UsagePlanId, is not set");
    return UpdateUsageOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [UsagePlanId]", false));
  }
  if (!request.KeyIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateUsage", "Required field: KeyId, is not set");
    return UpdateUsageOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [KeyId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateUsage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/usageplans/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetUsagePlanId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/keys/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetKeyId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/usage");
  return UpdateUsageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateUsageOutcomeCallable APIGatewayClient::UpdateUsageCallable(const UpdateUsageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateUsage, this, request, m_executor.get());
}

void APIGatewayClient::UpdateUsageAsync(const UpdateUsageRequest& request, const UpdateUsageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateUsage, this, request, handler, context, m_executor.get());
}

UpdateUsagePlanOutcome APIGatewayClient::UpdateUsagePlan(const UpdateUsagePlanRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateUsagePlan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.UsagePlanIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateUsagePlan", "Required field: UsagePlanId, is not set");
    return UpdateUsagePlanOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [UsagePlanId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateUsagePlan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/usageplans/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetUsagePlanId());
  return UpdateUsagePlanOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateUsagePlanOutcomeCallable APIGatewayClient::UpdateUsagePlanCallable(const UpdateUsagePlanRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateUsagePlan, this, request, m_executor.get());
}

void APIGatewayClient::UpdateUsagePlanAsync(const UpdateUsagePlanRequest& request, const UpdateUsagePlanResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateUsagePlan, this, request, handler, context, m_executor.get());
}

UpdateVpcLinkOutcome APIGatewayClient::UpdateVpcLink(const UpdateVpcLinkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateVpcLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.VpcLinkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateVpcLink", "Required field: VpcLinkId, is not set");
    return UpdateVpcLinkOutcome(Aws::Client::AWSError<APIGatewayErrors>(APIGatewayErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VpcLinkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateVpcLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vpclinks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVpcLinkId());
  return UpdateVpcLinkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateVpcLinkOutcomeCallable APIGatewayClient::UpdateVpcLinkCallable(const UpdateVpcLinkRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &APIGatewayClient::UpdateVpcLink, this, request, m_executor.get());
}

void APIGatewayClient::UpdateVpcLinkAsync(const UpdateVpcLinkRequest& request, const UpdateVpcLinkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&APIGatewayClient::UpdateVpcLink, this, request, handler, context, m_executor.get());
}

