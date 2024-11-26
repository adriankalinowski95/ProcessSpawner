import { BaseResponse } from "./base-response";

export interface ObjectsResponse<T> extends BaseResponse {
    objects: Array<T>;
}