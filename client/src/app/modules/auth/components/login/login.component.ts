import { Component, OnDestroy, OnInit } from '@angular/core';
import { shared } from '../../../shared/shared';
import { AuthService } from '../../services/auth.service';
import { FormBuilder, FormGroup, Validators } from '@angular/forms';
import { SubSink } from 'subsink';
import { ActivatedRoute, Router } from '@angular/router';
import { HttpClient } from '@angular/common/http';
import { first, Observable, tap } from 'rxjs';
import { UserDto } from '../../models/user-dto';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrl: './login.component.scss'
})
export class LoginComponent implements OnInit, OnDestroy{
    defaultAuth: any = {
        email: 'admin@gmail.com',
        password: 'admin123456',
    };
    
    public loginForm: FormGroup;
    hasError: boolean = false;
    returnUrl: string = "";
    isLoading$: Observable<boolean>;
     
    // private fields
    private subs: SubSink = new SubSink();
    
    constructor(
        private fb: FormBuilder,
        private authService: AuthService,
        private route: ActivatedRoute,
        private router: Router,
        private http: HttpClient,
        private notificationService: shared.services.NotificationService) {
        this.isLoading$ = this.authService.isLoading$;
        // redirect to home if already logged in
        if (this.authService.currentUserValue) {
          this.router.navigate(['/']);
        }

        this.loginForm = this.fb.group({
            email: [
                this.defaultAuth.email,
                Validators.compose([
                    Validators.required,
                    Validators.email,
                    Validators.minLength(3),
                    Validators.maxLength(320), // https://stackoverflow.com/questions/386294/what-is-the-maximum-length-of-a-valid-email-address
                ]),
            ],
            password: [
                this.defaultAuth.password,
                Validators.compose([
                    Validators.required,
                    Validators.minLength(3),
                    Validators.maxLength(100),
                ]),
                ],
            });
    }

    ngOnInit(): void {
        //this.initForm();
        // get return url from route parameters or default to '/'
        this.returnUrl = this.route.snapshot.queryParams['returnUrl'.toString()] || '/';
    }

    // convenience getter for easy access to form fields
    get form() {
        return this.loginForm.controls;
    }

    initForm() {
      this.loginForm = this.fb.group({
        email: [
            this.defaultAuth.email,
            Validators.compose([
                Validators.required,
                Validators.email,
                Validators.minLength(3),
                Validators.maxLength(320), // https://stackoverflow.com/questions/386294/what-is-the-maximum-length-of-a-valid-email-address
            ]),
        ],
        password: [
            this.defaultAuth.password,
            Validators.compose([
                Validators.required,
                Validators.minLength(3),
                Validators.maxLength(100),
            ]),
            ],
        });
    }

    submit() {
        this.hasError = false;
        const loginSubscr = this.authService.login(this.form["email"].value, this.form["password"].value).pipe(
            first(),
            tap((user: UserDto | undefined) => {
              if (user) {
                this.router.navigate([this.returnUrl]);
              } else {
                this.hasError = true;
              }
            })).subscribe();
        
        this.subs.add(loginSubscr);
    }

    ngOnDestroy() {
        this.subs.unsubscribe();
    }
}