import { Injectable, OnDestroy } from '@angular/core';
import { BehaviorSubject, catchError, finalize, first, map, Observable, of, retry, switchMap, tap } from 'rxjs';
import { UserDto } from '../models/user-dto';
import { environment } from '../../../../environments/environment.prod';
import { AuthHTTPService } from './auth-http.service';
import { Router } from '@angular/router';
import { shared } from '../../shared/shared';
import { TokenDto } from '../models/token-dto';

@Injectable({
  providedIn: 'root'
})
export class AuthService {
    private authLocalStorageToken = `${environment.appVersion}-${environment.USERDATA_KEY}`;

    currentUserSubject: BehaviorSubject<UserDto | undefined>;
    isLoadingSubject: BehaviorSubject<boolean>;

    currentUser$: Observable<UserDto | undefined>;
    isLoading$: Observable<boolean>;
  
    constructor(private authHttpService: AuthHTTPService, private router: Router) { 
        this.isLoadingSubject = new BehaviorSubject<boolean>(false);
        this.currentUserSubject = new BehaviorSubject<UserDto | undefined>(undefined);
        this.currentUser$ = this.currentUserSubject.asObservable();
        this.isLoading$ = this.isLoadingSubject.asObservable();
    }

    get currentUserValue(): UserDto | undefined{
        return this.currentUserSubject.value;
    }
    
    set currentUserValue(user: UserDto) {
        this.currentUserSubject.next(user);
    }
  
    login(
        email: string, 
        password: string, 
        responseCallback?: (response: shared.response.Object<any>) => void): Observable<UserDto | undefined> {
        
        this.isLoadingSubject.next(true);

        return this.authHttpService.login({ username:email, password: password }).pipe(
                first(),
                switchMap((tokenResponse: shared.response.Object<TokenDto>) => {
                    if (tokenResponse.status != shared.enums.BaseResponseStatus.Ok) {
                        if (!responseCallback) {
                            return of(undefined);
                        }

                        throw new Error(tokenResponse.errorMessage);
                    }

                    return this.authHttpService.getUserByToken(tokenResponse.object);
                }),
                switchMap((userResponse: shared.response.Object<UserDto>) => {
                    if (userResponse.status != shared.enums.BaseResponseStatus.Ok) {
                        if (!responseCallback) {
                            return of(undefined);
                        }

                        throw new Error(userResponse.errorMessage);
                    }

                    if (!this.setAuthFromLocalStorage(new UserDto(userResponse.object))) {
                        throw new Error("Error setting user data to local storage");

                        return of(undefined);
                    }

                    this.currentUserSubject.next(userResponse.object);

                    return of(userResponse.object);
                }),
                catchError((err) => {
                    if (responseCallback) {
                        responseCallback({
                            errorMessage: err.message, 
                            status: shared.enums.BaseResponseStatus.Error,
                            object: null
                        });
                    }

                    return of(undefined);
                }),
                finalize(() => {
                    this.isLoadingSubject.next(false)
                }));
    }

    logout(): void {    
        localStorage.removeItem(this.authLocalStorageToken);
        this.router.navigate(['/auth/login'], {
          queryParams: {},
        });
    }   

    getUserByToken(): Observable<UserDto | undefined> { 
        const user = this.getAuthFromLocalStorage();
        if (!UserDto.IsValid(user)) {
            this.logout();
        
            return of(undefined);
        }

        const token = user.token;
        if (!shared.isNotNullOrUndefined(user) || !shared.isNotNullOrUndefined(token)) {
            this.logout();
    
            return of(undefined);
        }
    
        this.isLoadingSubject.next(true);
    
        return this.authHttpService.getUserByToken(token).pipe(
            map((user: UserDto) => {
                if (user) {
                  this.currentUserSubject.next(user);
                } else {
                  this.logout();
                }

                return user;
            }),
            finalize(() => this.isLoadingSubject.next(false))
        );
    }

    private setAuthFromLocalStorage(userAuth: UserDto): boolean {
        if (shared.isNotNullOrUndefined(userAuth) && userAuth.token) {
            localStorage.setItem(this.authLocalStorageToken, JSON.stringify(userAuth));

            return true;
        }

        return false;
    }

    getAuthFromLocalStorage(): UserDto | undefined {
        try {
            const lsValue = localStorage.getItem(this.authLocalStorageToken);
            if (!lsValue) {
                return undefined;
            }

            const authData = JSON.parse(lsValue);
            
            return authData;
        } catch (error) {
            console.error(error);
            
            return undefined;
        }
    }
}
