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

#include <aws/lex-models/LexModelBuildingServiceClient.h>
#include <aws/lex-models/LexModelBuildingServiceErrorMarshaller.h>
#include <aws/lex-models/LexModelBuildingServiceEndpointProvider.h>
#include <aws/lex-models/model/CreateBotVersionRequest.h>
#include <aws/lex-models/model/CreateIntentVersionRequest.h>
#include <aws/lex-models/model/CreateSlotTypeVersionRequest.h>
#include <aws/lex-models/model/DeleteBotRequest.h>
#include <aws/lex-models/model/DeleteBotAliasRequest.h>
#include <aws/lex-models/model/DeleteBotChannelAssociationRequest.h>
#include <aws/lex-models/model/DeleteBotVersionRequest.h>
#include <aws/lex-models/model/DeleteIntentRequest.h>
#include <aws/lex-models/model/DeleteIntentVersionRequest.h>
#include <aws/lex-models/model/DeleteSlotTypeRequest.h>
#include <aws/lex-models/model/DeleteSlotTypeVersionRequest.h>
#include <aws/lex-models/model/DeleteUtterancesRequest.h>
#include <aws/lex-models/model/GetBotRequest.h>
#include <aws/lex-models/model/GetBotAliasRequest.h>
#include <aws/lex-models/model/GetBotAliasesRequest.h>
#include <aws/lex-models/model/GetBotChannelAssociationRequest.h>
#include <aws/lex-models/model/GetBotChannelAssociationsRequest.h>
#include <aws/lex-models/model/GetBotVersionsRequest.h>
#include <aws/lex-models/model/GetBotsRequest.h>
#include <aws/lex-models/model/GetBuiltinIntentRequest.h>
#include <aws/lex-models/model/GetBuiltinIntentsRequest.h>
#include <aws/lex-models/model/GetBuiltinSlotTypesRequest.h>
#include <aws/lex-models/model/GetExportRequest.h>
#include <aws/lex-models/model/GetImportRequest.h>
#include <aws/lex-models/model/GetIntentRequest.h>
#include <aws/lex-models/model/GetIntentVersionsRequest.h>
#include <aws/lex-models/model/GetIntentsRequest.h>
#include <aws/lex-models/model/GetMigrationRequest.h>
#include <aws/lex-models/model/GetMigrationsRequest.h>
#include <aws/lex-models/model/GetSlotTypeRequest.h>
#include <aws/lex-models/model/GetSlotTypeVersionsRequest.h>
#include <aws/lex-models/model/GetSlotTypesRequest.h>
#include <aws/lex-models/model/GetUtterancesViewRequest.h>
#include <aws/lex-models/model/ListTagsForResourceRequest.h>
#include <aws/lex-models/model/PutBotRequest.h>
#include <aws/lex-models/model/PutBotAliasRequest.h>
#include <aws/lex-models/model/PutIntentRequest.h>
#include <aws/lex-models/model/PutSlotTypeRequest.h>
#include <aws/lex-models/model/StartImportRequest.h>
#include <aws/lex-models/model/StartMigrationRequest.h>
#include <aws/lex-models/model/TagResourceRequest.h>
#include <aws/lex-models/model/UntagResourceRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::LexModelBuildingService;
using namespace Aws::LexModelBuildingService::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* LexModelBuildingServiceClient::SERVICE_NAME = "lex";
const char* LexModelBuildingServiceClient::ALLOCATION_TAG = "LexModelBuildingServiceClient";

LexModelBuildingServiceClient::LexModelBuildingServiceClient(const LexModelBuildingService::LexModelBuildingServiceClientConfiguration& clientConfiguration,
                                                             std::shared_ptr<LexModelBuildingServiceEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<LexModelBuildingServiceErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

LexModelBuildingServiceClient::LexModelBuildingServiceClient(const AWSCredentials& credentials,
                                                             std::shared_ptr<LexModelBuildingServiceEndpointProviderBase> endpointProvider,
                                                             const LexModelBuildingService::LexModelBuildingServiceClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<LexModelBuildingServiceErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

LexModelBuildingServiceClient::LexModelBuildingServiceClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                             std::shared_ptr<LexModelBuildingServiceEndpointProviderBase> endpointProvider,
                                                             const LexModelBuildingService::LexModelBuildingServiceClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<LexModelBuildingServiceErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  LexModelBuildingServiceClient::LexModelBuildingServiceClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<LexModelBuildingServiceErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<LexModelBuildingServiceEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

LexModelBuildingServiceClient::LexModelBuildingServiceClient(const AWSCredentials& credentials,
                                                             const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<LexModelBuildingServiceErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<LexModelBuildingServiceEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

LexModelBuildingServiceClient::LexModelBuildingServiceClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                             const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<LexModelBuildingServiceErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<LexModelBuildingServiceEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
LexModelBuildingServiceClient::~LexModelBuildingServiceClient()
{
}

std::shared_ptr<LexModelBuildingServiceEndpointProviderBase>& LexModelBuildingServiceClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void LexModelBuildingServiceClient::init(const LexModelBuildingService::LexModelBuildingServiceClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Lex Model Building Service");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void LexModelBuildingServiceClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CreateBotVersionOutcome LexModelBuildingServiceClient::CreateBotVersion(const CreateBotVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateBotVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateBotVersion", "Required field: Name, is not set");
    return CreateBotVersionOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateBotVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/versions");
  return CreateBotVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateBotVersionOutcomeCallable LexModelBuildingServiceClient::CreateBotVersionCallable(const CreateBotVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::CreateBotVersion, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::CreateBotVersionAsync(const CreateBotVersionRequest& request, const CreateBotVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::CreateBotVersion, this, request, handler, context, m_executor.get());
}

CreateIntentVersionOutcome LexModelBuildingServiceClient::CreateIntentVersion(const CreateIntentVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateIntentVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateIntentVersion", "Required field: Name, is not set");
    return CreateIntentVersionOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateIntentVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/intents/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/versions");
  return CreateIntentVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateIntentVersionOutcomeCallable LexModelBuildingServiceClient::CreateIntentVersionCallable(const CreateIntentVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::CreateIntentVersion, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::CreateIntentVersionAsync(const CreateIntentVersionRequest& request, const CreateIntentVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::CreateIntentVersion, this, request, handler, context, m_executor.get());
}

CreateSlotTypeVersionOutcome LexModelBuildingServiceClient::CreateSlotTypeVersion(const CreateSlotTypeVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSlotTypeVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateSlotTypeVersion", "Required field: Name, is not set");
    return CreateSlotTypeVersionOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSlotTypeVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/slottypes/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/versions");
  return CreateSlotTypeVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateSlotTypeVersionOutcomeCallable LexModelBuildingServiceClient::CreateSlotTypeVersionCallable(const CreateSlotTypeVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::CreateSlotTypeVersion, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::CreateSlotTypeVersionAsync(const CreateSlotTypeVersionRequest& request, const CreateSlotTypeVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::CreateSlotTypeVersion, this, request, handler, context, m_executor.get());
}

DeleteBotOutcome LexModelBuildingServiceClient::DeleteBot(const DeleteBotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteBot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteBot", "Required field: Name, is not set");
    return DeleteBotOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteBot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  return DeleteBotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteBotOutcomeCallable LexModelBuildingServiceClient::DeleteBotCallable(const DeleteBotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::DeleteBot, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::DeleteBotAsync(const DeleteBotRequest& request, const DeleteBotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::DeleteBot, this, request, handler, context, m_executor.get());
}

DeleteBotAliasOutcome LexModelBuildingServiceClient::DeleteBotAlias(const DeleteBotAliasRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteBotAlias, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteBotAlias", "Required field: Name, is not set");
    return DeleteBotAliasOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.BotNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteBotAlias", "Required field: BotName, is not set");
    return DeleteBotAliasOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BotName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteBotAlias, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetBotName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/aliases/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  return DeleteBotAliasOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteBotAliasOutcomeCallable LexModelBuildingServiceClient::DeleteBotAliasCallable(const DeleteBotAliasRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::DeleteBotAlias, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::DeleteBotAliasAsync(const DeleteBotAliasRequest& request, const DeleteBotAliasResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::DeleteBotAlias, this, request, handler, context, m_executor.get());
}

DeleteBotChannelAssociationOutcome LexModelBuildingServiceClient::DeleteBotChannelAssociation(const DeleteBotChannelAssociationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteBotChannelAssociation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteBotChannelAssociation", "Required field: Name, is not set");
    return DeleteBotChannelAssociationOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.BotNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteBotChannelAssociation", "Required field: BotName, is not set");
    return DeleteBotChannelAssociationOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BotName]", false));
  }
  if (!request.BotAliasHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteBotChannelAssociation", "Required field: BotAlias, is not set");
    return DeleteBotChannelAssociationOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BotAlias]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteBotChannelAssociation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetBotName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/aliases/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetBotAlias());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  return DeleteBotChannelAssociationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteBotChannelAssociationOutcomeCallable LexModelBuildingServiceClient::DeleteBotChannelAssociationCallable(const DeleteBotChannelAssociationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::DeleteBotChannelAssociation, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::DeleteBotChannelAssociationAsync(const DeleteBotChannelAssociationRequest& request, const DeleteBotChannelAssociationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::DeleteBotChannelAssociation, this, request, handler, context, m_executor.get());
}

DeleteBotVersionOutcome LexModelBuildingServiceClient::DeleteBotVersion(const DeleteBotVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteBotVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteBotVersion", "Required field: Name, is not set");
    return DeleteBotVersionOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.VersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteBotVersion", "Required field: Version, is not set");
    return DeleteBotVersionOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Version]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteBotVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/versions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVersion());
  return DeleteBotVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteBotVersionOutcomeCallable LexModelBuildingServiceClient::DeleteBotVersionCallable(const DeleteBotVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::DeleteBotVersion, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::DeleteBotVersionAsync(const DeleteBotVersionRequest& request, const DeleteBotVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::DeleteBotVersion, this, request, handler, context, m_executor.get());
}

DeleteIntentOutcome LexModelBuildingServiceClient::DeleteIntent(const DeleteIntentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteIntent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteIntent", "Required field: Name, is not set");
    return DeleteIntentOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteIntent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/intents/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  return DeleteIntentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteIntentOutcomeCallable LexModelBuildingServiceClient::DeleteIntentCallable(const DeleteIntentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::DeleteIntent, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::DeleteIntentAsync(const DeleteIntentRequest& request, const DeleteIntentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::DeleteIntent, this, request, handler, context, m_executor.get());
}

DeleteIntentVersionOutcome LexModelBuildingServiceClient::DeleteIntentVersion(const DeleteIntentVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteIntentVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteIntentVersion", "Required field: Name, is not set");
    return DeleteIntentVersionOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.VersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteIntentVersion", "Required field: Version, is not set");
    return DeleteIntentVersionOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Version]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteIntentVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/intents/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/versions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVersion());
  return DeleteIntentVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteIntentVersionOutcomeCallable LexModelBuildingServiceClient::DeleteIntentVersionCallable(const DeleteIntentVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::DeleteIntentVersion, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::DeleteIntentVersionAsync(const DeleteIntentVersionRequest& request, const DeleteIntentVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::DeleteIntentVersion, this, request, handler, context, m_executor.get());
}

DeleteSlotTypeOutcome LexModelBuildingServiceClient::DeleteSlotType(const DeleteSlotTypeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteSlotType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteSlotType", "Required field: Name, is not set");
    return DeleteSlotTypeOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteSlotType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/slottypes/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  return DeleteSlotTypeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteSlotTypeOutcomeCallable LexModelBuildingServiceClient::DeleteSlotTypeCallable(const DeleteSlotTypeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::DeleteSlotType, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::DeleteSlotTypeAsync(const DeleteSlotTypeRequest& request, const DeleteSlotTypeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::DeleteSlotType, this, request, handler, context, m_executor.get());
}

DeleteSlotTypeVersionOutcome LexModelBuildingServiceClient::DeleteSlotTypeVersion(const DeleteSlotTypeVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteSlotTypeVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteSlotTypeVersion", "Required field: Name, is not set");
    return DeleteSlotTypeVersionOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.VersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteSlotTypeVersion", "Required field: Version, is not set");
    return DeleteSlotTypeVersionOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Version]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteSlotTypeVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/slottypes/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/version/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVersion());
  return DeleteSlotTypeVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteSlotTypeVersionOutcomeCallable LexModelBuildingServiceClient::DeleteSlotTypeVersionCallable(const DeleteSlotTypeVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::DeleteSlotTypeVersion, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::DeleteSlotTypeVersionAsync(const DeleteSlotTypeVersionRequest& request, const DeleteSlotTypeVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::DeleteSlotTypeVersion, this, request, handler, context, m_executor.get());
}

DeleteUtterancesOutcome LexModelBuildingServiceClient::DeleteUtterances(const DeleteUtterancesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteUtterances, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.BotNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteUtterances", "Required field: BotName, is not set");
    return DeleteUtterancesOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BotName]", false));
  }
  if (!request.UserIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteUtterances", "Required field: UserId, is not set");
    return DeleteUtterancesOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [UserId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteUtterances, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetBotName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/utterances/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetUserId());
  return DeleteUtterancesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteUtterancesOutcomeCallable LexModelBuildingServiceClient::DeleteUtterancesCallable(const DeleteUtterancesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::DeleteUtterances, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::DeleteUtterancesAsync(const DeleteUtterancesRequest& request, const DeleteUtterancesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::DeleteUtterances, this, request, handler, context, m_executor.get());
}

GetBotOutcome LexModelBuildingServiceClient::GetBot(const GetBotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetBot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetBot", "Required field: Name, is not set");
    return GetBotOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.VersionOrAliasHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetBot", "Required field: VersionOrAlias, is not set");
    return GetBotOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VersionOrAlias]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetBot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/versions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVersionOrAlias());
  return GetBotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetBotOutcomeCallable LexModelBuildingServiceClient::GetBotCallable(const GetBotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetBot, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetBotAsync(const GetBotRequest& request, const GetBotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetBot, this, request, handler, context, m_executor.get());
}

GetBotAliasOutcome LexModelBuildingServiceClient::GetBotAlias(const GetBotAliasRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetBotAlias, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetBotAlias", "Required field: Name, is not set");
    return GetBotAliasOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.BotNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetBotAlias", "Required field: BotName, is not set");
    return GetBotAliasOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BotName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetBotAlias, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetBotName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/aliases/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  return GetBotAliasOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetBotAliasOutcomeCallable LexModelBuildingServiceClient::GetBotAliasCallable(const GetBotAliasRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetBotAlias, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetBotAliasAsync(const GetBotAliasRequest& request, const GetBotAliasResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetBotAlias, this, request, handler, context, m_executor.get());
}

GetBotAliasesOutcome LexModelBuildingServiceClient::GetBotAliases(const GetBotAliasesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetBotAliases, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.BotNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetBotAliases", "Required field: BotName, is not set");
    return GetBotAliasesOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BotName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetBotAliases, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetBotName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/aliases/");
  return GetBotAliasesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetBotAliasesOutcomeCallable LexModelBuildingServiceClient::GetBotAliasesCallable(const GetBotAliasesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetBotAliases, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetBotAliasesAsync(const GetBotAliasesRequest& request, const GetBotAliasesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetBotAliases, this, request, handler, context, m_executor.get());
}

GetBotChannelAssociationOutcome LexModelBuildingServiceClient::GetBotChannelAssociation(const GetBotChannelAssociationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetBotChannelAssociation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetBotChannelAssociation", "Required field: Name, is not set");
    return GetBotChannelAssociationOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.BotNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetBotChannelAssociation", "Required field: BotName, is not set");
    return GetBotChannelAssociationOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BotName]", false));
  }
  if (!request.BotAliasHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetBotChannelAssociation", "Required field: BotAlias, is not set");
    return GetBotChannelAssociationOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BotAlias]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetBotChannelAssociation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetBotName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/aliases/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetBotAlias());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  return GetBotChannelAssociationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetBotChannelAssociationOutcomeCallable LexModelBuildingServiceClient::GetBotChannelAssociationCallable(const GetBotChannelAssociationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetBotChannelAssociation, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetBotChannelAssociationAsync(const GetBotChannelAssociationRequest& request, const GetBotChannelAssociationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetBotChannelAssociation, this, request, handler, context, m_executor.get());
}

GetBotChannelAssociationsOutcome LexModelBuildingServiceClient::GetBotChannelAssociations(const GetBotChannelAssociationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetBotChannelAssociations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.BotNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetBotChannelAssociations", "Required field: BotName, is not set");
    return GetBotChannelAssociationsOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BotName]", false));
  }
  if (!request.BotAliasHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetBotChannelAssociations", "Required field: BotAlias, is not set");
    return GetBotChannelAssociationsOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BotAlias]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetBotChannelAssociations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetBotName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/aliases/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetBotAlias());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  return GetBotChannelAssociationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetBotChannelAssociationsOutcomeCallable LexModelBuildingServiceClient::GetBotChannelAssociationsCallable(const GetBotChannelAssociationsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetBotChannelAssociations, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetBotChannelAssociationsAsync(const GetBotChannelAssociationsRequest& request, const GetBotChannelAssociationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetBotChannelAssociations, this, request, handler, context, m_executor.get());
}

GetBotVersionsOutcome LexModelBuildingServiceClient::GetBotVersions(const GetBotVersionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetBotVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetBotVersions", "Required field: Name, is not set");
    return GetBotVersionsOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetBotVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/versions/");
  return GetBotVersionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetBotVersionsOutcomeCallable LexModelBuildingServiceClient::GetBotVersionsCallable(const GetBotVersionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetBotVersions, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetBotVersionsAsync(const GetBotVersionsRequest& request, const GetBotVersionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetBotVersions, this, request, handler, context, m_executor.get());
}

GetBotsOutcome LexModelBuildingServiceClient::GetBots(const GetBotsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetBots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetBots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  return GetBotsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetBotsOutcomeCallable LexModelBuildingServiceClient::GetBotsCallable(const GetBotsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetBots, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetBotsAsync(const GetBotsRequest& request, const GetBotsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetBots, this, request, handler, context, m_executor.get());
}

GetBuiltinIntentOutcome LexModelBuildingServiceClient::GetBuiltinIntent(const GetBuiltinIntentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetBuiltinIntent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SignatureHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetBuiltinIntent", "Required field: Signature, is not set");
    return GetBuiltinIntentOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Signature]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetBuiltinIntent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/builtins/intents/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetSignature());
  return GetBuiltinIntentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetBuiltinIntentOutcomeCallable LexModelBuildingServiceClient::GetBuiltinIntentCallable(const GetBuiltinIntentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetBuiltinIntent, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetBuiltinIntentAsync(const GetBuiltinIntentRequest& request, const GetBuiltinIntentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetBuiltinIntent, this, request, handler, context, m_executor.get());
}

GetBuiltinIntentsOutcome LexModelBuildingServiceClient::GetBuiltinIntents(const GetBuiltinIntentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetBuiltinIntents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetBuiltinIntents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/builtins/intents/");
  return GetBuiltinIntentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetBuiltinIntentsOutcomeCallable LexModelBuildingServiceClient::GetBuiltinIntentsCallable(const GetBuiltinIntentsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetBuiltinIntents, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetBuiltinIntentsAsync(const GetBuiltinIntentsRequest& request, const GetBuiltinIntentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetBuiltinIntents, this, request, handler, context, m_executor.get());
}

GetBuiltinSlotTypesOutcome LexModelBuildingServiceClient::GetBuiltinSlotTypes(const GetBuiltinSlotTypesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetBuiltinSlotTypes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetBuiltinSlotTypes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/builtins/slottypes/");
  return GetBuiltinSlotTypesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetBuiltinSlotTypesOutcomeCallable LexModelBuildingServiceClient::GetBuiltinSlotTypesCallable(const GetBuiltinSlotTypesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetBuiltinSlotTypes, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetBuiltinSlotTypesAsync(const GetBuiltinSlotTypesRequest& request, const GetBuiltinSlotTypesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetBuiltinSlotTypes, this, request, handler, context, m_executor.get());
}

GetExportOutcome LexModelBuildingServiceClient::GetExport(const GetExportRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetExport, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExport", "Required field: Name, is not set");
    return GetExportOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.VersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExport", "Required field: Version, is not set");
    return GetExportOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Version]", false));
  }
  if (!request.ResourceTypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExport", "Required field: ResourceType, is not set");
    return GetExportOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceType]", false));
  }
  if (!request.ExportTypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExport", "Required field: ExportType, is not set");
    return GetExportOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ExportType]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetExport, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/exports/");
  return GetExportOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetExportOutcomeCallable LexModelBuildingServiceClient::GetExportCallable(const GetExportRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetExport, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetExportAsync(const GetExportRequest& request, const GetExportResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetExport, this, request, handler, context, m_executor.get());
}

GetImportOutcome LexModelBuildingServiceClient::GetImport(const GetImportRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetImport, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ImportIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetImport", "Required field: ImportId, is not set");
    return GetImportOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ImportId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetImport, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/imports/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetImportId());
  return GetImportOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetImportOutcomeCallable LexModelBuildingServiceClient::GetImportCallable(const GetImportRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetImport, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetImportAsync(const GetImportRequest& request, const GetImportResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetImport, this, request, handler, context, m_executor.get());
}

GetIntentOutcome LexModelBuildingServiceClient::GetIntent(const GetIntentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetIntent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetIntent", "Required field: Name, is not set");
    return GetIntentOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.VersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetIntent", "Required field: Version, is not set");
    return GetIntentOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Version]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetIntent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/intents/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/versions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVersion());
  return GetIntentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetIntentOutcomeCallable LexModelBuildingServiceClient::GetIntentCallable(const GetIntentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetIntent, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetIntentAsync(const GetIntentRequest& request, const GetIntentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetIntent, this, request, handler, context, m_executor.get());
}

GetIntentVersionsOutcome LexModelBuildingServiceClient::GetIntentVersions(const GetIntentVersionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetIntentVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetIntentVersions", "Required field: Name, is not set");
    return GetIntentVersionsOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetIntentVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/intents/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/versions/");
  return GetIntentVersionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetIntentVersionsOutcomeCallable LexModelBuildingServiceClient::GetIntentVersionsCallable(const GetIntentVersionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetIntentVersions, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetIntentVersionsAsync(const GetIntentVersionsRequest& request, const GetIntentVersionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetIntentVersions, this, request, handler, context, m_executor.get());
}

GetIntentsOutcome LexModelBuildingServiceClient::GetIntents(const GetIntentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetIntents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetIntents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/intents/");
  return GetIntentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetIntentsOutcomeCallable LexModelBuildingServiceClient::GetIntentsCallable(const GetIntentsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetIntents, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetIntentsAsync(const GetIntentsRequest& request, const GetIntentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetIntents, this, request, handler, context, m_executor.get());
}

GetMigrationOutcome LexModelBuildingServiceClient::GetMigration(const GetMigrationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetMigration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MigrationIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetMigration", "Required field: MigrationId, is not set");
    return GetMigrationOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MigrationId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetMigration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/migrations/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMigrationId());
  return GetMigrationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetMigrationOutcomeCallable LexModelBuildingServiceClient::GetMigrationCallable(const GetMigrationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetMigration, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetMigrationAsync(const GetMigrationRequest& request, const GetMigrationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetMigration, this, request, handler, context, m_executor.get());
}

GetMigrationsOutcome LexModelBuildingServiceClient::GetMigrations(const GetMigrationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetMigrations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetMigrations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/migrations");
  return GetMigrationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetMigrationsOutcomeCallable LexModelBuildingServiceClient::GetMigrationsCallable(const GetMigrationsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetMigrations, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetMigrationsAsync(const GetMigrationsRequest& request, const GetMigrationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetMigrations, this, request, handler, context, m_executor.get());
}

GetSlotTypeOutcome LexModelBuildingServiceClient::GetSlotType(const GetSlotTypeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSlotType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetSlotType", "Required field: Name, is not set");
    return GetSlotTypeOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.VersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetSlotType", "Required field: Version, is not set");
    return GetSlotTypeOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Version]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSlotType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/slottypes/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/versions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVersion());
  return GetSlotTypeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetSlotTypeOutcomeCallable LexModelBuildingServiceClient::GetSlotTypeCallable(const GetSlotTypeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetSlotType, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetSlotTypeAsync(const GetSlotTypeRequest& request, const GetSlotTypeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetSlotType, this, request, handler, context, m_executor.get());
}

GetSlotTypeVersionsOutcome LexModelBuildingServiceClient::GetSlotTypeVersions(const GetSlotTypeVersionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSlotTypeVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetSlotTypeVersions", "Required field: Name, is not set");
    return GetSlotTypeVersionsOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSlotTypeVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/slottypes/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/versions/");
  return GetSlotTypeVersionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetSlotTypeVersionsOutcomeCallable LexModelBuildingServiceClient::GetSlotTypeVersionsCallable(const GetSlotTypeVersionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetSlotTypeVersions, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetSlotTypeVersionsAsync(const GetSlotTypeVersionsRequest& request, const GetSlotTypeVersionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetSlotTypeVersions, this, request, handler, context, m_executor.get());
}

GetSlotTypesOutcome LexModelBuildingServiceClient::GetSlotTypes(const GetSlotTypesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSlotTypes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSlotTypes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/slottypes/");
  return GetSlotTypesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetSlotTypesOutcomeCallable LexModelBuildingServiceClient::GetSlotTypesCallable(const GetSlotTypesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetSlotTypes, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetSlotTypesAsync(const GetSlotTypesRequest& request, const GetSlotTypesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetSlotTypes, this, request, handler, context, m_executor.get());
}

GetUtterancesViewOutcome LexModelBuildingServiceClient::GetUtterancesView(const GetUtterancesViewRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetUtterancesView, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.BotNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetUtterancesView", "Required field: BotName, is not set");
    return GetUtterancesViewOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BotName]", false));
  }
  if (!request.BotVersionsHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetUtterancesView", "Required field: BotVersions, is not set");
    return GetUtterancesViewOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BotVersions]", false));
  }
  if (!request.StatusTypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetUtterancesView", "Required field: StatusType, is not set");
    return GetUtterancesViewOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StatusType]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetUtterancesView, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetBotName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/utterances");
  ss.str("?view=aggregation");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return GetUtterancesViewOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetUtterancesViewOutcomeCallable LexModelBuildingServiceClient::GetUtterancesViewCallable(const GetUtterancesViewRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::GetUtterancesView, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::GetUtterancesViewAsync(const GetUtterancesViewRequest& request, const GetUtterancesViewResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::GetUtterancesView, this, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome LexModelBuildingServiceClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable LexModelBuildingServiceClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::ListTagsForResource, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::ListTagsForResource, this, request, handler, context, m_executor.get());
}

PutBotOutcome LexModelBuildingServiceClient::PutBot(const PutBotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutBot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutBot", "Required field: Name, is not set");
    return PutBotOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutBot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/versions/$LATEST");
  return PutBotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PutBotOutcomeCallable LexModelBuildingServiceClient::PutBotCallable(const PutBotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::PutBot, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::PutBotAsync(const PutBotRequest& request, const PutBotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::PutBot, this, request, handler, context, m_executor.get());
}

PutBotAliasOutcome LexModelBuildingServiceClient::PutBotAlias(const PutBotAliasRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutBotAlias, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutBotAlias", "Required field: Name, is not set");
    return PutBotAliasOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.BotNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutBotAlias", "Required field: BotName, is not set");
    return PutBotAliasOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [BotName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutBotAlias, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bots/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetBotName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/aliases/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  return PutBotAliasOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PutBotAliasOutcomeCallable LexModelBuildingServiceClient::PutBotAliasCallable(const PutBotAliasRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::PutBotAlias, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::PutBotAliasAsync(const PutBotAliasRequest& request, const PutBotAliasResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::PutBotAlias, this, request, handler, context, m_executor.get());
}

PutIntentOutcome LexModelBuildingServiceClient::PutIntent(const PutIntentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutIntent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutIntent", "Required field: Name, is not set");
    return PutIntentOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutIntent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/intents/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/versions/$LATEST");
  return PutIntentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PutIntentOutcomeCallable LexModelBuildingServiceClient::PutIntentCallable(const PutIntentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::PutIntent, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::PutIntentAsync(const PutIntentRequest& request, const PutIntentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::PutIntent, this, request, handler, context, m_executor.get());
}

PutSlotTypeOutcome LexModelBuildingServiceClient::PutSlotType(const PutSlotTypeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutSlotType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutSlotType", "Required field: Name, is not set");
    return PutSlotTypeOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutSlotType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/slottypes/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/versions/$LATEST");
  return PutSlotTypeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PutSlotTypeOutcomeCallable LexModelBuildingServiceClient::PutSlotTypeCallable(const PutSlotTypeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::PutSlotType, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::PutSlotTypeAsync(const PutSlotTypeRequest& request, const PutSlotTypeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::PutSlotType, this, request, handler, context, m_executor.get());
}

StartImportOutcome LexModelBuildingServiceClient::StartImport(const StartImportRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartImport, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartImport, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/imports/");
  return StartImportOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartImportOutcomeCallable LexModelBuildingServiceClient::StartImportCallable(const StartImportRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::StartImport, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::StartImportAsync(const StartImportRequest& request, const StartImportResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::StartImport, this, request, handler, context, m_executor.get());
}

StartMigrationOutcome LexModelBuildingServiceClient::StartMigration(const StartMigrationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartMigration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartMigration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/migrations");
  return StartMigrationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartMigrationOutcomeCallable LexModelBuildingServiceClient::StartMigrationCallable(const StartMigrationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::StartMigration, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::StartMigrationAsync(const StartMigrationRequest& request, const StartMigrationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::StartMigration, this, request, handler, context, m_executor.get());
}

TagResourceOutcome LexModelBuildingServiceClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable LexModelBuildingServiceClient::TagResourceCallable(const TagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::TagResource, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::TagResource, this, request, handler, context, m_executor.get());
}

UntagResourceOutcome LexModelBuildingServiceClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<LexModelBuildingServiceErrors>(LexModelBuildingServiceErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable LexModelBuildingServiceClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &LexModelBuildingServiceClient::UntagResource, this, request, m_executor.get());
}

void LexModelBuildingServiceClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&LexModelBuildingServiceClient::UntagResource, this, request, handler, context, m_executor.get());
}

