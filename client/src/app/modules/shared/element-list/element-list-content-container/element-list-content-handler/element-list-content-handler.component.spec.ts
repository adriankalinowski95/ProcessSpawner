import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ElementListContentHandlerComponent } from './element-list-content-handler.component';

describe('ElementListContentHandlerComponent', () => {
  let component: ElementListContentHandlerComponent;
  let fixture: ComponentFixture<ElementListContentHandlerComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ElementListContentHandlerComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ElementListContentHandlerComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
