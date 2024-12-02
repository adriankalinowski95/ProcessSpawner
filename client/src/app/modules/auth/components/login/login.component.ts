import { Component, ElementRef, OnDestroy, OnInit, ViewChild } from '@angular/core';
import { shared } from '../../../shared/shared';
import { AuthService } from '../../services/auth.service';
import { FormBuilder, FormGroup, Validators } from '@angular/forms';
import { SubSink } from 'subsink';
import { ActivatedRoute, Router } from '@angular/router';
import { HttpClient } from '@angular/common/http';
import { first, Observable, tap } from 'rxjs';
import { UserDto } from '../../models/user-dto';
import { TokenDto } from '../../models/token-dto';

interface KeyValuePair {
    key: number;
    value: string;
}

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrl: './login.component.scss'
})
export class LoginComponent implements OnInit, OnDestroy {
    private subs: SubSink = new SubSink();
    private validationMessages = {
        email: {
            required: 'Email is required!'
        },
        password: {
            required: 'Password is required'
        }
    };
    private returnUrl: string = "";

    defaultAuth: any = {
        email: 'admin@gmail.com',
        password: 'admin123456',
    };
    
    public loginForm: FormGroup;
    public formErrorHandlerService: shared.services.FormErrorHandling;
    public lastValidateMessage: string = "";
    public isLoading$: Observable<boolean>;
    
    constructor(
        private fb: FormBuilder,
        private authService: AuthService,
        private route: ActivatedRoute,
        private router: Router) {
        this.isLoading$ = this.authService.isLoading$;

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
                    Validators.maxLength(320),
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

        this.formErrorHandlerService = new shared.services.FormErrorHandling(this.loginForm, this.validationMessages);
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
        this.lastValidateMessage = "";

        const errorHandler = (response: shared.response.Object<any>) => {
            this.lastValidateMessage = response.errorMessage.toString();
        };

        this.authService.login(this.form["email"].value, this.form["password"].value, errorHandler).pipe(
            first(),
            tap((user: UserDto | undefined) => {
                if (shared.isNotNullOrUndefined(user)) {
                    this.router.navigate([this.returnUrl]);
                }
            })).subscribe(
        );
    }

    ngOnDestroy() {
        this.subs.unsubscribe();
    }
}